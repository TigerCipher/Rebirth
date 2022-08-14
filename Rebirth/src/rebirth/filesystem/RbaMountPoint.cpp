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
// File Name: RbaMountPoint.cpp
// Date File Created: 8/3/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"
#include "RbaMountPoint.h"
#include "rebirth/util/FileUtil.h"
#include "rebirth/util/StringUtil.h"

#include "RbaFile.h"

namespace rebirth
{
	bool RbaMountPoint::OnMount()
	{
		RB_CORE_INFO("Creating mount point for RBA file {}", mPhysicalPath);
		mRbaFile = createScope<PhysicalFile>(mPhysicalPath, FileMode_Read);

		if (!mRbaFile->Open())
		{
			RB_CORE_ERROR("Failed to open RBA file {}", mPhysicalPath);
			return false;
		}

		//if (!mRbaFile->ReadFrom(&mHeader, 0, sizeof(RbaHeader)))
		if (!mRbaFile->Read(&mHeader, sizeof(RbaHeader)))
		{
			RB_CORE_ERROR("Failed to read RBA file header");
			return false;
		}

		if (strcmp(mHeader.id, "RBA") != 0)
		{
			RB_CORE_ERROR("Bad format for RBA file");
			return false;
		}
		if (mHeader.rbaVersion != RBA_NEWEST_VERSION)
		{
			RB_CORE_ERROR("RBA File [{}] version does not match the RBA version used by this engine (found {}, expected {})",
				mPhysicalPath, mHeader.rbaVersion, RBA_NEWEST_VERSION);
			return false;
		}

		mOrder = mHeader.contentVersion;


		mFileEntries.resize(mHeader.numEntries);
		//if(!mRbaFile->ReadFrom(mFileEntries.data(), sizeof(RbaHeader), mHeader.numEntries * sizeof(RbaFileEntry)))
		if(!mRbaFile->Read(mFileEntries.data(), mHeader.numEntries * sizeof(RbaFileEntry)))
		{
			RB_CORE_ERROR("Failed to read RBA file entries");
			return false;
		}

		for(auto& entry : mFileEntries)
		{
			std::string normal = file::NormalizePath(entry.filepath);
			memcpy(entry.filepath, normal.data(), normal.size());
		}

		std::sort(mFileEntries.begin(), mFileEntries.end(), [](const RbaFileEntry& a, const RbaFileEntry& b)
			{
				return a.filepath > b.filepath;
			});

		return true;
	}

	bool RbaMountPoint::HasFile(const std::string& filePath) const
	{
		const std::string resolved = ToLower(filePath);

		return std::find_if(mFileEntries.begin(), mFileEntries.end(), [&resolved](const RbaFileEntry& entry)
			{
				return entry.filepath == resolved;
			}) != mFileEntries.end();
	}

	Scope<File> RbaMountPoint::GetFile(const std::string& filePath)
	{
		auto it = std::find_if(mFileEntries.begin(), mFileEntries.end(), [&filePath](const RbaFileEntry& entry)
			{
				return entry.filepath == filePath;
			});

		if (it == mFileEntries.end())
			return nullptr;
		return createScope<RbaFile>(filePath, this, it._Ptr);
	}
}

