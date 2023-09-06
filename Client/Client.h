#pragma once
#include <queue>
#include <filesystem>
#include "Log.h"
#include "File.h"
#include "User.h"
#include "UserIO.h"
#include "Socket.h"
#include "Package.h"
#include "Requests.h"
#include "RequestMaker.h"
#include "ResponseExtractor.h"
namespace fs = std::filesystem;

namespace Andrew
{
	class FileSharingClient
	{
	public:
		FileSharingClient(const int& domain, const int& service, const int& protocol,
			const int& port, const char* host, const std::string& logPath)
			: wsaStartup(WSAStartup(MAKEWORD(2, 2), &ws)),
			connectingSocket(domain, service, protocol, port, host),
			io(PayloadSize)
		{
			log = new FileLog(logPath);
			if (wsaStartup < 0) exit(EXIT_FAILURE);
		}

		bool auth()
		{
			sender(makeRequest.auth());

			Package response = accepter();
			Response::Auth payload = extractResponse.auth(response.payload);
			user.auth = payload.status;
			user.id = payload.userId;

			return user.auth;
		}
		void uploadFile(const std::string& path)
		{
			std::string name = fs::path(path).filename().string();
			File file;
			file.bRead(path);
			u_long picSize = file.getSize();
			u_long common = sizeof(Request::SendFileChunk::bin);
			u_long chunks = getChunksCount(picSize, common);

			Response::LobbyReg lobbyRegRes = createLobby(file.getSize(), chunks, name.c_str());
			while (lobbyRegRes.status == 0) lobbyRegRes = createLobby(file.getSize(), chunks, name.c_str());

			u_long lobbyId = lobbyRegRes.lobbyId;
			io.writeln("File created, id: " + std::to_string(lobbyId));
			io.writeln("Uploading started");

			//how many percents is already uploaded
			//status bar
			int loadstep = 20, loadprogress = 0;
			for (int chunkId = 0; chunkId < chunks; chunkId++)
			{
				bool sent = 0;
				while (!sent)
				{
					sender(makeRequest.sendFileChunk(
						user.id,
						lobbyId,
						chunkId,
						getChunkSize(picSize, common, chunks, chunkId),
						getChunkOffset(common, chunkId),
						file.getBuffer()
					));
					Response::SendFileChunk payload = extractResponse.sendFileChunk(accepter().payload);

					if (payload.status == 1) sent = 1;
				}

				if (chunkId == (int)(loadprogress / 100.0 * (chunks - 1)))
				{
					io.write("Uploading: " + std::to_string(loadprogress) + "% completed\n");
					loadprogress += (loadprogress + loadstep <= 100) ? loadstep : 100 - loadprogress;
				}
			}

		}
		void downloadFile(const std::string& path, int lobbyId)
		{
			Response::LobbyConnect lobbyConnectRes = joinLobby(lobbyId);
			std::string name(lobbyConnectRes.dataName, lobbyConnectRes.nameSize);
			io.writeln("Downloading: " + name);
			u_long picSize = lobbyConnectRes.dataSize;
			u_long common = sizeof(Request::SendFileChunk::bin);
			u_long chunks = getChunksCount(picSize, common);

			char* tmp = new char[picSize];

			//how many percents is already uploaded
			//status bar
			int loadstep = 20, loadprogress = 0;
			for (int chunkId = 0; chunkId < chunks; chunkId++)
			{
				bool downloaded = 0;
				while (!downloaded)
				{
					sender(makeRequest.getFileChunk(
						lobbyId,
						chunkId
					));
					Response::GetFileChunk payload = extractResponse.getFileChunk(accepter().payload);

					/** putting pic chunk to memory
					*/
					memcpy(tmp + payload.offset, payload.bin, payload.size);

					if (payload.status == 1) downloaded = 1;
				}
				if (chunkId == (int)(loadprogress / 100.0 * (chunks - 1)))
				{
					io.write("Downloding: " + std::to_string(loadprogress) + "%\n");
					loadprogress += (loadprogress + loadstep <= 100) ? loadstep : 100 - loadprogress;
				}
			}

			File file(ByteBuffer(tmp, picSize));
			file.bWrite(path);
		}

	private:
		/*
		*	at start constructor call WSAStartup()
		*	then socketconstructor. IMPORTANT!!!
		*/
		int wsaStartup = -1;
		ConnectingSocket connectingSocket;
		/** THESE ^^^
		*/
		UserIO io;
		User user;
		WSADATA ws;

		Log* log = nullptr;
		bool closing = 0;

		RequestMaker makeRequest;
		ResponseExtractor extractResponse;
	
		Response::LobbyReg createLobby(u_long picSize, u_long chunks, const char* name)
		{
			io.writeln("Start creating a file...");
			sender(makeRequest.lobbyReg(
				user.id,
				picSize,
				strlen(name),
				chunks,
				name
			));
			return extractResponse.lobbyReg(accepter().payload);
		}
		Response::LobbyConnect joinLobby(u_long lobbyId)
		{
			sender(makeRequest.lobbyConnect(
				user.id,
				lobbyId
			));
			return extractResponse.lobbyConnect(accepter().payload);
		}

		Package accepter()
		{
			Package response;
			int netres = recv(connectingSocket.getSock(), (char*)&response, sizeof(response), 0);
			if (netres > 0)
			{
				log->write("==== Recv: " + std::to_string(netres) + " bytes ====\n");
			}
			else if (netres == 0)
			{
				log->write("==== Connection closed ====\n");
			}
			else
			{
				log->write("==== recv failed with error: " + WSAGetLastError());
				log->write(" ====\n");
				closesocket(connectingSocket.getSock());
			}
			
			return response;
		}
		void sender(const Package& request)
		{
			int netres = send(connectingSocket.getSock(), (char*)&request, sizeof(request), 0);
			if (netres > 0)
			{
				log->write("==== Send: " + std::to_string(netres) + " bytes ====\n");
			}
			else if (netres == 0)
			{
				log->write("==== Connection closed ====\n");
			}
			else
			{
				log->write("==== send failed with error: " + WSAGetLastError());
				log->write(" ====\n");
				closesocket(connectingSocket.getSock());
			}			
		}
		void shutdownSend()
		{
			int netres = shutdown(connectingSocket.getSock(), SD_SEND);
			if (netres < 0)
			{
				log->write("==== shutdown failed with error: " + WSAGetLastError());
				log->write(" ====\n");
				closesocket(connectingSocket.getSock());
			}
		}
		void shutdownRecv()
		{
			int netres = shutdown(connectingSocket.getSock(), SD_RECEIVE);
			if (netres < 0)
			{
				log->write("==== shutdown failed with error: " + WSAGetLastError());
				log->write(" ====\n");
				closesocket(connectingSocket.getSock());
			}
		}
		/** File chunks handlers
		*/
		u_long getChunksCount(u_long size, u_long payloadSize)
		{
			u_long res = size / payloadSize;
			if (res % payloadSize > 0) res++;
			return res;
		}
		u_long getChunkSize(u_long whole, u_long common, u_long chunks, u_long chunkId)
		{
			u_long chunkSize = chunkId == chunks - 1
				? whole - common * chunkId : common;
			return chunkSize;
		}
		u_long getChunkOffset(u_long common, u_long chunkId)
		{
			return chunkId * common;
		}
	};
}