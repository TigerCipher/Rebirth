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
// File Name: RbaFile.cpp
// Date File Created: 8/3/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"
#include "RbaFile.h"

#include "RbaMountPoint.h"

#include <zlib.h>

namespace rebirth
{
	size_t RbaFile::Read(void* dest, size_t size)
	{
		if (!mMountPoint)
			return 0;

		const RbaMountPoint* mntPnt = mMountPoint;

		const size_t sizeToRead = mFilePointer + size > mFileEntry->uncompressedSize ? mFileEntry->uncompressedSize - mFilePointer : size;

		// End of file
		if (sizeToRead <= 0)
			return 0;

		size_t read;
		if(mFileEntry->compressed)
		{
			if (!CacheUncompressedData())
				return 0;
			memcpy(dest, mUncompressedCache.data() + mFilePointer, sizeToRead);
			read = sizeToRead;
		}
		else
		{
			read = mntPnt->GetRbaFile()->ReadFrom(dest, mFileEntry->offset + mFilePointer, sizeToRead);
		}

		mFilePointer += read;
		return read;
	}

	size_t RbaFile::Write(const void* data, size_t size)
	{
		throw std::logic_error("Not implemented");
	}

	bool RbaFile::Flush()
	{
		throw std::logic_error("Not implemented");
	}

	bool RbaFile::Close()
	{
		mUncompressedCache.clear();
		return true;
	}

	bool RbaFile::IsOpen() const
	{
		return mFileEntry && mMountPoint && mMountPoint->GetRbaFile();
	}

	bool RbaFile::OpenRead()
	{
		mSize = mFileEntry->uncompressedSize;
		return true;
	}

	bool RbaFile::OpenWrite()
	{
		throw std::logic_error("Not implemented");
	}

	bool RbaFile::CacheUncompressedData()
	{
		if(mUncompressedCache.empty())
		{
			if (!mMountPoint)
				return false;
			const RbaMountPoint* mntPnt = mMountPoint;

			std::vector<char> tempBuffer(mFileEntry->compressedSize);
			if(!mMountPoint->GetRbaFile()->ReadFrom(tempBuffer.data(), mFileEntry->offset, mFileEntry->compressedSize))
			{
				return false;
			}

			if(!mFileEntry->compressed)
			{
				tempBuffer.swap(mUncompressedCache);
			}

			mUncompressedCache.resize(mFileEntry->uncompressedSize);

			if(uncompress((byte*)mUncompressedCache.data(), &mFileEntry->uncompressedSize, (byte*)tempBuffer.data(), tempBuffer.size()) != Z_OK)
			{
				return false;
			}
		}

		return true;
	}
}

