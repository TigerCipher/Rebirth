// ------------------------------------------------------------------------------
// 
// Rebirth
//    Copyright 2022 Matthew Rogers
// 
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
// 
//        http://www.apache.org/licenses/LICENSE-2.0
// 
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
// 
// File Name: IOStream.cpp
// Date File Created: 8/2/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"
#include "IOStream.h"
#include "rebirth/util/MathUtil.h"

namespace rebirth
{

	////////////////////////////////////////////////////////////////////////////
	// InputStream Implementations /////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	size_t InputStream::ReadFrom(void* dest, size_t from, size_t size)
	{
		SetPointer(from);
		return Read(dest, size);
	}

	bool InputStream::ReadAllBytes(std::vector<byte>& buffer)
	{
		SetPointer(0);
		const size_t size = GetSize();
		buffer.resize(size);
		return Read(buffer.data(), buffer.size()) == size;
	}

	bool InputStream::MovePointer(int delta)
	{
		return SetPointer(mFilePointer + delta);
	}

	std::string InputStream::ReadSizedString()
	{
		uint8 size;
		Read(&size, sizeof(uint8));
		std::string str;
		str.resize(size);
		Read(&str[0], size);
		return str;
	}

	std::string InputStream::ReadString()
	{
		std::stringstream ss;
		while (!IsEndOfFile())
		{
			char c = ReadChar();
			if (c == '\0') return ss.str();
			ss << c;
		}

		return ss.str();
	}

	int InputStream::ReadInt()
	{
		int out;
		return Read(&out, sizeof(int)) == sizeof(int) ? out : 0;
	}

	int64 InputStream::ReadInt64()
	{
		int64 out;
		return Read(&out, sizeof(int64)) == sizeof(int64) ? out : 0;
	}

	uint32 InputStream::ReadUnsigned()
	{
		uint32 out;
		return Read(&out, sizeof(uint32)) == sizeof(uint32) ? out : 0;
	}

	float InputStream::ReadFloat()
	{
		float out;
		return Read(&out, sizeof(float)) == sizeof(float) ? out : 0.0f;
	}

	double InputStream::ReadDouble()
	{
		double out;
		return Read(&out, sizeof(double)) == sizeof(double) ? out : 0.0;
	}

	char InputStream::ReadChar()
	{
		char out;
		return Read(&out, sizeof(char)) == sizeof(char) ? out : 0;
	}

	byte InputStream::ReadByte()
	{
		byte out;
		return Read(&out, sizeof(byte)) == sizeof(byte) ? out : 0;
	}

	bool InputStream::GetLine(std::string& out, char delimiter)
	{
		static constexpr int bufferSize = 50;
		out.resize(bufferSize);
		uint32 currentLength = 0;

		// while(true) // Visual studio seems to give annoying warning for constant expression while loops
		for (;;)
		{
			// Reached end of file
			if (const size_t read = Read(&out[currentLength], 1); read == 0)
			{
				out.resize(currentLength);
				return currentLength > 0;
			}

			// Windows newlines are \r\n
			if (out[currentLength] == '\r')
				continue;
			if (out[currentLength] == delimiter)
			{
				out.resize(currentLength);
				return true;
			}

			++currentLength;

			if (currentLength >= bufferSize)
				out.resize(currentLength + bufferSize);
		}
	}

	bool InputStream::SetPointer(size_t position)
	{
		mFilePointer = position;
		return true;
	}

	bool InputStream::SetPointerFromEnd(size_t position)
	{
		if (position > mSize)
			return false;
		mFilePointer = mSize - position;
		return true;
	}




	////////////////////////////////////////////////////////////////////////////
	// OutputStream Implementations ////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////


	bool OutputStream::WriteString(const std::string& str)
	{
		return Write(str.data(), str.size() + 1) == str.size() + 1;
	}

	bool OutputStream::WriteSizedString(const std::string& str)
	{
		if (!WriteByte((byte)str.size()))
			return false;
		if (!Write(str.data(), str.size()))
			return false;
		return true;
	}

	bool OutputStream::WriteLine(const std::string& line)
	{
#ifdef RB_PLATFORM_WINDOWS
		return Write(line.c_str(), line.size()) && WriteByte((byte)'\r') && WriteByte((byte)'\n');
#else
		return Write(line.c_str(), line.size()) && WriteByte((byte)'\n');
#endif
	}



	////////////////////////////////////////////////////////////////////////////
	// MemoryStream Implementations ////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////



	size_t MemoryStream::Write(const void* data, const size_t size)
	{
		RB_CORE_ASSERT(mFilePointer + size < mSize);

		memcpy(Current(), data, size);
		mFilePointer += size;

		return size;
	}

	size_t MemoryStream::Read(void* dest, const size_t size)
	{
		const size_t s = math::Clamp(size, mSize, (size_t)0);
		memcpy(dest, Current(), s);
		mFilePointer += s;
		return s;
	}

	void* MemoryStream::Current() const
	{
		return (char*)mBuffer + mFilePointer;
	}
}

