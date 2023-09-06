#pragma once
#include "Types.h"

namespace Andrew
{
	const int PayloadSize = 512;

	enum class PckgType
	{
		PingReq,
		PingRes,

		AuthReq,
		AuthRes,

		LobbiesGetReq,
		LobbiesGetRes,

		LobbyRegReq,
		LobbyRegRes,

		LobbyConnectReq,
		LobbyConnectRes,

		SendFileChunkReq,
		SendFileChunkRes,

		GetFileChunkReq,
		GetFileChunkRes,
	};

	struct Package
	{
		PckgType type;
		char* payload[PayloadSize];
	};
}
