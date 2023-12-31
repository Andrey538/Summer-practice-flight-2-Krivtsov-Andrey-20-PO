#pragma once
#include <string>
#include "ByteBuffer.h"

namespace Andrew
{
	class FileHandler
	{
	public:
		FileHandler(const std::string& fileName, const std::string& savePath,
			u_long bufferSize)
			:	bufferSize(bufferSize),
				fileName(fileName),
				savePath(savePAth)
		{
			buffer = new char[bufferSize];
		}
		~FileHandler()
		{
			delete[] buffer;
		}
	private:
		ByteBuffer buffer;
		std::string fileName;
		std::string savePath;
	};
}