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

enum FileMode : rebirth::uint8
{
	FileMode_Read = BIT(0),
	FileMode_Write = BIT(1),
	FileMode_Append = BIT(2)
};

namespace rebirth
{

	// #TODO: Currently unused. Things are BINARY by default, and may very well stay that way
	enum class ContentType
	{
		TEXT,
		BINARY
	};

	class File : public IOStream
	{
	public:
		File(const std::string& filename, const uint8 flags) :
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

		/**
		 * \brief Opens the file in either write mode or read mode depending on the filemode flags set. It can be opened in both, and if so, it will internally call OpenWrite
		 * \return True if the file was able to be opened, false if it failed
		 */
		bool Open();

		virtual bool Flush() = 0;
		virtual bool Close() = 0;
		virtual bool IsOpen() const = 0;

	protected:
		virtual bool OpenRead() = 0;
		virtual bool OpenWrite() = 0;

		uint8 mFileMode;
	};
}

