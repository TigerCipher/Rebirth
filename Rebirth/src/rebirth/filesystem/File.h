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
// File Name: File.h
// Date File Created: 8/2/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "IOStream.h"
#include "rebirth/util/FileUtil.h"

#include <string>

namespace rebirth
{
	enum FileMode : uint8
	{
		FileMode_None = BIT(0),
		FileMode_Read = BIT(1),
		FileMode_Write = BIT(2),
		FileMode_Append = BIT(3)
	};

	enum class ContentType
	{
		TEXT,
		BINARY
	};

	class File : public IOStream
	{
	public:
		File(const std::string& filename, const uint8 flags = FileMode_None) :
			mFileMode(flags)
		{
			mSource = filename;
		}

		virtual ~File() = default;

		bool Exists() const
		{
			return file::Exists(mSource);
		}

		std::string GetDirectoryPath() const
		{
			return file::GetDirectoryPath(mSource);
		}

		std::string GetFileName() const
		{
			return file::GetFileName(mSource);
		}

		void SetFileMode(const FileMode flags) { mFileMode = flags; }

		virtual bool OpenRead() = 0;
		virtual bool OpenWrite() = 0;
		virtual bool Flush() = 0;
		virtual bool Close() = 0;
		virtual bool IsOpen() const = 0;

	protected:
		uint8 mFileMode;
	};
}

