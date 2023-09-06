#pragma once
#include "Package.h"
#include "Requests.h"

namespace Andrew
{
	/** Casts network data to host data
	*/
	class ResponseExtractor
	{
	public:
		ResponseExtractor() = default;

		Response::Auth auth(char* const* payload)
		{
			Response::Auth res;
			memcpy(&res, payload, sizeof(res));

			res.userId = ntohl(res.userId);

			return res;
		}
		Response::LobbiesGet lobbiesGet(char* const* payload) {
			Response::LobbiesGetInfo info;
			memcpy(&info, payload, sizeof(info));

			info.length = ntohl(info.length);

			Response::LobbiesGet res;
			memcpy(&res, &info, sizeof(info));

			res.lobbies = new Response::LobbyGet[info.length];

			char* const* j = payload + sizeof(info);
			for (int i = 0; i < info.length; i++) {
				Response::LobbyGet& lobby = res.lobbies[i];
				
				memcpy(&lobby.id, j, sizeof(lobby.id)); j += sizeof(lobby.id);
				lobby.id = ntohl(lobby.id);
				memcpy(&lobby.dataNameSize, j, sizeof(lobby.dataNameSize)); j += sizeof(lobby.dataNameSize);
				lobby.dataNameSize = ntohl(lobby.dataNameSize);
				memcpy(&lobby.dataName, j, lobby.dataNameSize); j += lobby.dataNameSize;
			}

			return res;
		}
		Response::LobbyReg lobbyReg(char* const* payload)
		{
			Response::LobbyReg res;
			memcpy(&res, payload, sizeof(res));

			res.lobbyId = ntohl(res.lobbyId);

			return res;
		}
		Response::LobbyConnect lobbyConnect(char* const* payload)
		{
			Response::LobbyConnect res;
			memcpy(&res, payload, sizeof(res));

			res.dataSize = ntohl(res.dataSize);
			res.nameSize = ntohl(res.nameSize);

			return res;
		}
		Response::SendFileChunk sendFileChunk(char* const* payload)
		{
			Response::SendFileChunk res;
			memcpy(&res, payload, sizeof(res));

			res.chunkId = ntohl(res.chunkId);

			return res;
		}
		Response::GetFileChunk getFileChunk(char* const* payload)
		{
			Response::GetFileChunk res;
			memcpy(&res, payload, sizeof(res));

			res.chunkId = ntohl(res.chunkId);
			res.size	= ntohl(res.size);
			res.offset	= ntohl(res.offset);

			return res;
		}
	};
}