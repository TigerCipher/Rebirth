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
// File Name: InputStream.h
// Date File Created: 8/2/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once


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

	private:
		size_t mSize = 0;
		size_t mFilePointer = 0;
		std::string mSource{};
	};
}
