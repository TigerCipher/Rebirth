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
// File Name: RbaMountPoint.h
// Date File Created: 8/3/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "MountPoint.h"
#include "RbaFileHeader.h"
#include "PhysicalFile.h"

namespace rebirth
{
	class RbaMountPoint : public MountPoint
	{
	public:
		RbaMountPoint(const std::string& physicalPath) :
			MountPoint(physicalPath, -1) {}
		virtual ~RbaMountPoint() = default;

		bool OnMount() override;
		bool HasFile(const std::string& filePath) const override;
		Scope<File> GetFile(const std::string& filePath) override;

		PhysicalFile* GetRbaFile() const { return mRbaFile.get(); }
	private:
		Scope<PhysicalFile> mRbaFile;
		RbaHeader mHeader{};
		std::vector<RbaFileEntry> mFileEntries;
	};
}

