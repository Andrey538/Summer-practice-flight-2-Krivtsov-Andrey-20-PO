#pragma once
#include "Types.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

namespace Andrew
{
	class File
	{
	public:

		File() = default;
		File(wchar_t* data, u_long size, const std::string& filename)
			: filename(filename), data(data), size(size)
		{
		}

		void read(const std::string& path)
		{
			std::wifstream in(path, std::ios::in);
			mRead(path, in);
			in.close();
		}
		void write(const std::string& path)
		{
			std::wofstream out(path, std::ios::out);
			mWrite(path, out);
			out.close();
		}
		void bRead(const std::string& path)
		{
			std::wifstream in(path, std::ios::in | std::ios::binary);
			mRead(path, in);
			in.close();
		}

		void bWrite(const std::string& path)
		{
			std::wofstream out(path, std::ios::out | std::ios::binary);
			mWrite(path, out);
			out.close();
		}
		const wchar_t* getBuffer() const { return data; }
		int getSize() const { return size; }
		std::string getFilename() const { return filename; }

	protected:
		void mRead(const std::string& path, std::wifstream& in)
		{
			this->path = fs::path(path);
			this->filename = this->path.filename().string();

			in.seekg(0, std::ios::end);
			size = in.tellg();
			if (size == -1)
			{
				std::cout << "Can't read the file, path: " << path << '\n';
				return;
			}
			in.seekg(0, std::ios::beg);
			data = new wchar_t[size];
			in.read(data, size);
		}
		void mWrite(const std::string& path, std::wofstream& out)
		{
			out.write(data, (long long)size * sizeof(*data));
		}
	public:
		wchar_t* data;
		u_long size;
		fs::path path;
		std::string filename;
	};
}