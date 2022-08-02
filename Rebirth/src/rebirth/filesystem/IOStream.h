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
// File Name: IOStream.h
// Date File Created: 8/2/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#define RB_OUTPUTSTREAM_WRITE(name, type) bool Write##name(const type value) { return Write(&value, sizeof(type)) == sizeof(type); } 

namespace rebirth
{
	class InputStream
	{
	public:
		InputStream() = default;
		InputStream(const size_t size) : mSize(size) {}

		virtual ~InputStream() = default;
		size_t ReadFrom(void* dest, size_t from, size_t size);
		bool ReadAllBytes(std::vector<byte>& buffer);
		bool MovePointer(int delta);

		std::string ReadSizedString();
		std::string ReadString();
		int ReadInt();
		int64 ReadInt64();
		uint32 ReadUnsigned();
		float ReadFloat();
		double ReadDouble();
		char ReadChar();
		byte ReadByte();

		bool GetLine(std::string& out, char delimiter = '\n');

		bool IsEndOfFile() const { return mFilePointer >= mSize; }
		size_t GetSize() const { return mSize; }
		size_t GetPointer() const { return mFilePointer; }

		void SetSource(const std::string& src) { mSource = src; }
		const std::string& GetSource() const { return mSource; }

		virtual bool SetPointer(size_t position);
		virtual bool SetPointerFromEnd(size_t position);

		virtual size_t Read(void* dest, size_t size) = 0;

	protected:
		size_t mSize = 0;
		size_t mFilePointer = 0;
		std::string mSource{};
	};


	class OutputStream
	{
	public:
		OutputStream() = default;
		virtual ~OutputStream() = default;

		virtual size_t Write(const void* data, size_t size) = 0;

		bool WriteString(const std::string& str);
		bool WriteSizedString(const std::string& str);
		bool WriteLine(const std::string& line);

		RB_OUTPUTSTREAM_WRITE(Int, int)
		RB_OUTPUTSTREAM_WRITE(Int64, int64)
		RB_OUTPUTSTREAM_WRITE(Float, float)
		RB_OUTPUTSTREAM_WRITE(Double, double)
		RB_OUTPUTSTREAM_WRITE(Char, char)
		RB_OUTPUTSTREAM_WRITE(Byte, byte)
	};


	class IOStream : public InputStream, OutputStream
	{
	public:
		IOStream() = default;
		IOStream(const size_t size) : InputStream(size) {}

		virtual ~IOStream() = default;
	};



	// Not sure if I'll actually need this, but might as well add just in case
	class MemoryStream : public IOStream
	{
	public:
		MemoryStream() = default;
		MemoryStream(void* data, const size_t size) : IOStream(size), mBuffer(data) {}

		virtual ~MemoryStream() = default;

		size_t Write(const void* data, size_t size) override;
		size_t Read(void* dest, size_t size) override;
	private:
		void* Current() const;


		void* mBuffer = nullptr;
	};

}

