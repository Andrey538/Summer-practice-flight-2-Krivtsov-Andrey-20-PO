#pragma once
#include <iostream>
#include <fstream>
#include <string>

namespace Andrew
{
	class Log
	{
	public:
		virtual void write(const char* buffer, uint64_t size) = 0;
		virtual void write(const std::string& buffer) {
			write(buffer.c_str(), buffer.size());
		}
	};

	class FileLog : public Log
	{
	public:
		FileLog(const std::string& path)
			: path(path)
		{
			out.open(path, std::ios::app | std::ios::out);
		}

		~FileLog() {
			out.close();
		}
		void write(const char* buffer, uint64_t size) override
		{			
			std::cout << buffer;
			out.write(buffer, size);			
		}
	private:
		std::string path;
		std::fstream out;
	};
}