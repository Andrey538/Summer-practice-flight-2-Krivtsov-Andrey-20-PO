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
		std::vector<LobbyDto> lobbiesGet(char* const* payload) {
			Response::LobbiesGet response;
			memcpy(&response, payload, sizeof(response));

			response.length = ntohl(response.length);

			std::vector<LobbyDto> res(response.length);

			char* j = (char*)response.lobbies;
			for (int i = 0; i < response.length; i++) {
				LobbyDto& lobby = res[i];
				
				memcpy(&lobby.id, j, sizeof(Response::LobbyGet::id)); 
				j += sizeof(Response::LobbyGet::id);
				lobby.id = ntohl(lobby.id);

				u_long nameSize = 0;
				memcpy(&nameSize, j, sizeof(Response::LobbyGet::dataNameSize)); 
				j += sizeof(Response::LobbyGet::dataNameSize);
				nameSize = ntohl(nameSize);

				lobby.dataName = std::string(j, nameSize);
				j += nameSize;				
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