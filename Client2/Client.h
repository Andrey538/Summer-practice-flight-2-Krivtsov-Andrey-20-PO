#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
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
#include "rsa_encrypt_decrypt.h"
namespace fs = std::filesystem;

namespace Andrew
{
	class FileSharingClient
	{
	public:
		FileSharingClient(const int& domain, const int& service, const int& protocol,
			const int& serverPort, const std::string& serverHost, const std::string& logPath)
		{
			wsaStartup = WSAStartup(MAKEWORD(2, 2), &ws);
			host = GetIPv4Address();
			connectingSocket = ConnectingSocket(domain, service, protocol, serverPort, serverHost.c_str());
			io = UserIO(PayloadSize);
			log = new FileLog(logPath);
			if (wsaStartup < 0) exit(EXIT_FAILURE);
		}

		~FileSharingClient() {
			closesocket(connectingSocket.getSock());
			WSACleanup();
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
		void uploadFile(File& file)
		{			
			u_long filesize = file.getSize(); //wchars count
			u_long common = sizeof(Request::SendFileChunk::bin); // bin size
			u_long chunks = getChunksCount(filesize * sizeof(*File::data), common); // 982 bytes => 462 wchar_t

			Response::LobbyReg lobbyRegRes = createLobby(file.getSize(), chunks, file.getFilename());
			while (lobbyRegRes.status == 0) lobbyRegRes = createLobby(file.getSize(), chunks, file.getFilename());

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
						getChunkSize(filesize, common, chunks, chunkId),
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
		File downloadFile(int lobbyId)
		{
			Response::LobbyConnect lobbyConnectRes = joinLobby(lobbyId);
			std::string name(lobbyConnectRes.dataName, lobbyConnectRes.nameSize);
			io.writeln("Downloading: " + name);
			u_long filesize = lobbyConnectRes.dataSize;
			u_long common = sizeof(Request::SendFileChunk::bin);
			u_long chunks = getChunksCount(filesize * sizeof(*File::data), common);

			wchar_t* tmp = new wchar_t[filesize];

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

					memcpy(tmp + payload.offset, &payload.bin, payload.size);

					if (payload.status == 1) downloaded = 1;
				}
				if (chunkId == (int)(loadprogress / 100.0 * (chunks - 1)))
				{
					io.write("Downloding: " + std::to_string(loadprogress) + "%\n");
					loadprogress += (loadprogress + loadstep <= 100) ? loadstep : 100 - loadprogress;
				}
			}

			File file(tmp, filesize, name);
			return file;
		}

		std::vector<LobbyDto> getLobbies() {
			sender(makeRequest.lobbiesGet());
			return extractResponse.lobbiesGet(accepter().payload);
		}

		std::string getHost() const {
			return host;
		}

	private:
		int wsaStartup = -1;
		ConnectingSocket connectingSocket;
		UserIO io;
		User user;
		WSADATA ws;
		std::string host;
		Log* log = nullptr;
		bool closing = 0;

		RequestMaker makeRequest;
		ResponseExtractor extractResponse;
	
		Response::LobbyReg createLobby(u_long filesize, u_long chunks, std::string name)
		{
			io.writeln("Start creating a file...");
			sender(makeRequest.lobbyReg(
				user.id,
				filesize,
				name.size(),
				chunks,
				name.c_str()
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
			if (size <= payloadSize) return 1;
			u_long res = size / (payloadSize);
			if (res % payloadSize > 0) res++;
			return res;
		}
		u_long getChunkSize(u_long whole, u_long common, u_long chunks, u_long chunkId)
		{
			u_long chunkSize = chunkId == chunks - 1
				? (whole * sizeof(*File::data)) - common * chunkId 
				: common;
			return chunkSize;
		}
		u_long getChunkOffset(u_long common, u_long chunkId)
		{
			return chunkId * (common / sizeof(*File::data));
		}
		std::string GetIPv4Address() {
			std::string ip_address = "";

			char hostname[256];
			gethostname(hostname, sizeof(hostname));
			if (gethostname(hostname, sizeof(hostname)) == 0) {
				struct hostent* host = gethostbyname(hostname);
				if (host != nullptr) {
					struct in_addr** addr_list = reinterpret_cast<struct in_addr**>(host->h_addr_list);
					for (int i = 0; addr_list[i] != nullptr; ++i) {
						ip_address = inet_ntoa(*addr_list[i]);
						break;
					}
				}
			}

			return ip_address;
		}
	};
}