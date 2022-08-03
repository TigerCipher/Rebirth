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
// File Name: PhysicalFile.h
// Date File Created: 8/2/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "File.h"


#define INVALID_HANDLE ((FileHandle)(int64)-1)

namespace rebirth
{
	using FileHandle = void*;

	class PhysicalFile : public File
	{
	public:
		PhysicalFile(const std::string& filename, const uint8 flags = FileMode_None) : File(filename, flags), mPhysicalPath(filename) {}
		virtual ~PhysicalFile();

		bool SetPointer(size_t position) override;
		bool SetPointerFromEnd(size_t position) override;
		size_t Read(void* dest, size_t size) override;
		size_t Write(const void* data, size_t size) override;
		bool OpenRead() override;
		bool OpenWrite() override;
		bool Flush() override;
		bool Close() override;
		bool IsOpen() const override;
		
	private:
		static bool CreateDirectoryTree(const std::string& path);
		bool SetPointerInternal(size_t position, bool fromEnd);

		std::string mPhysicalPath;
		FileHandle mHandle = INVALID_HANDLE;
	};
}

