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
// File Name: RbaFile.h
// Date File Created: 8/3/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "File.h"
#include "RbaFileHeader.h"

namespace rebirth
{
	class RbaMountPoint;

	class RbaFile : public File
	{
	public:
		RbaFile(const std::string& filename, RbaMountPoint* mountPoint, RbaFileEntry* fileEntry)
			: File(filename, FileMode_Read), mMountPoint(mountPoint), mFileEntry(fileEntry) {}

		virtual ~RbaFile() = default;

		size_t Read(void* dest, size_t size) override;
		size_t Write(const void* data, size_t size) override;
		bool Flush() override;
		bool Close() override;
		bool IsOpen() const override;
	protected:
		bool OpenRead() override;
		bool OpenWrite() override;

	private:
		bool CacheUncompressedData();

		RbaMountPoint* mMountPoint;
		RbaFileEntry* mFileEntry;

		std::vector<char> mUncompressedCache;
	};
}

