#pragma once
#include "Types.h"
#include "User.h"
#include <string>
#include "Registrator.h"

namespace Andrew
{
	class Lobby
	{
	public:
		Lobby() = default;
		Lobby(u_long id, u_long ownerId, u_long dataSize, u_long chunks, const std::string& dataName)
			: id(id), ownerId(ownerId), size(dataSize), dataName(dataName), chunks(chunks)
		{
			data = new wchar_t[size];
		}
	public:
		u_long id;
		u_long ownerId;
		u_long chunks;
		wchar_t* data;
		u_long size;
		std::string dataName;
		double lifetime = 0.0f;
		bool uploaded = 0;
	};
}