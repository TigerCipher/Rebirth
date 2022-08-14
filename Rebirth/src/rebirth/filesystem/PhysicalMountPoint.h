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
// File Name: PhysicalMountPoint.h
// Date File Created: 8/13/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "MountPoint.h"

namespace rebirth
{

	class PhysicalMountPoint : public MountPoint
	{
	public:
		PhysicalMountPoint(const std::string& physicalPath) :
			MountPoint(physicalPath, 0) {}
		virtual ~PhysicalMountPoint() = default;

		bool OnMount() override;
		bool HasFile(const std::string& filePath) const override;
		Scope<File> GetFile(const std::string& filePath) override;

	private:
		bool RegisterDirectory(const std::string& path);

		std::vector<std::string> mFileEntries;
	};
}

