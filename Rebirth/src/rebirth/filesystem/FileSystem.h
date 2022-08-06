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
// File Name: FileSystem.h
// Date File Created: 8/6/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "MountPoint.h"
#include "PhysicalFile.h"
#include "RbaMountPoint.h"
#include "RbaFile.h"

namespace rebirth
{
	class FileSystem
	{
	public:

		using MountPointPtr = Scope<MountPoint>;

		static bool Mount(const std::string& physicalPath);
		static void AddRbaLocation(const std::string& path);
		static Scope<File> GetFile(const std::string& filename);

		// file attribute utilities? Modified times, creation times, etc?

	private:
		static MountPointPtr CreateMountPoint(const std::string& physicalPath);

		static std::vector<MountPointPtr> mMountPoints;
		static std::vector<std::string> mRbaLocations;
	};
}

