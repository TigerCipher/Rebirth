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
// File Name: FileSystem.cpp
// Date File Created: 8/6/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"
#include "FileSystem.h"

#include "PhysicalMountPoint.h"
#include "RbaMountPoint.h"
#include "RbaFile.h"

namespace rebirth
{

	std::vector<FileSystem::MountPointPtr> FileSystem::mMountPoints;
	std::vector<std::string> FileSystem::mRbaLocations;

	bool FileSystem::Mount(const std::string& physicalPath)
	{
		MountPointPtr ptr = CreateMountPoint(file::FixPath(physicalPath));
		if (!ptr)
		{
			RB_CORE_ERROR("Failed to mount {}", physicalPath);
			return false;
		}

		if (!ptr->OnMount())
			return false;

		mMountPoints.push_back(std::move(ptr));

		std::sort(mMountPoints.begin(), mMountPoints.end(), [](const MountPointPtr& a, const MountPointPtr& b)
			{
				return a->GetOrder() > b->GetOrder();
			});

		RB_CORE_INFO("Mounted {} successfully", physicalPath);
		return true;
	}

	void FileSystem::AddRbaLocation(const std::string& path)
	{
		if (std::find(mRbaLocations.begin(), mRbaLocations.end(), path) != mRbaLocations.end())
			return;
		mRbaLocations.push_back(path);

		for(const auto& locs : mRbaLocations)
		{
			if (std::filesystem::is_directory(locs))
			{
				for (auto& dirEntry : std::filesystem::recursive_directory_iterator(locs))
				{
					if (is_directory(dirEntry))
						continue;
					std::vector<std::string> rbaFiles;
					if (file::GetFileExtensionFromPath(dirEntry.path()) == "rba")
					{
						Mount(dirEntry.path().string());
					}
				}
			}
			else
			{
				Mount(locs);
			}
		}
	}

	Scope<File> FileSystem::GetFile(const std::string& filename)
	{
		const std::string path = file::NormalizePath(filename);
		for(const auto& mntPnt : mMountPoints)
		{
			if (Scope<File> file = mntPnt->GetFile(path))
				return file;
		}

		return nullptr;
	}

	FileSystem::MountPointPtr FileSystem::CreateMountPoint(const std::string& physicalPath)
	{
		const std::string ext = file::GetFileExtensionFromString(physicalPath);
		if(ext.length() == 0)
			return createScope<PhysicalMountPoint>(physicalPath);
		if (ext == "rba")
			return createScope<RbaMountPoint>(physicalPath);

		return nullptr;
	}

}

