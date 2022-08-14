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
// File Name: PhysicalMountPoint.cpp
// Date File Created: 8/13/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"
#include "PhysicalMountPoint.h"
#include "PhysicalFile.h"

namespace rebirth
{
	bool PhysicalMountPoint::OnMount()
	{
		if (!RegisterDirectory(mPhysicalPath))
			return false;
		std::sort(mFileEntries.begin(), mFileEntries.end(), [](const std::string& a, const std::string& b)
			{
				return a > b;
			});

		return true;
	}

	bool PhysicalMountPoint::HasFile(const std::string& filePath) const
	{
		const std::string resolved = file::NormalizePath(mPhysicalPath + "/" + filePath);
		return std::find_if(mFileEntries.begin(), mFileEntries.end(), [&resolved](const std::string& entry)
			{
				return entry == resolved;
			}) != mFileEntries.end();
	}

	Scope<File> PhysicalMountPoint::GetFile(const std::string& filePath)
	{
		const std::string resolved = file::NormalizePath(mPhysicalPath + "/" + filePath);
		auto it = std::find_if(mFileEntries.begin(), mFileEntries.end(), [&resolved](const std::string& entry)
			{
				return strcmp(entry.c_str(), resolved.c_str()) == 0;
			});
		if (it == mFileEntries.end())
			return nullptr;
		Scope<PhysicalFile> file = createScope<PhysicalFile>(resolved, FileMode_Read);
		file->SetSource(filePath);
		return file;
	}

	bool PhysicalMountPoint::RegisterDirectory(const std::string& path)
	{
		// #CONSIDER Once again... using windows only code here... do I *really* give a flying monkey about supporting non windows?
		WIN32_FIND_DATAA findData;
		auto handle = FindFirstFileA((path + "\\*").c_str(), &findData);
		if (handle == INVALID_HANDLE_VALUE)
			return false;
		do
		{
			if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
					continue;
				if(!RegisterDirectory(path + "/" + findData.cFileName))
				{
					FindClose(handle);
					return false;
				}
			}
			else
			{
				mFileEntries.push_back(file::NormalizePath(path + "/" + findData.cFileName));
			}
		}
		while (FindNextFileA(handle, &findData));

		FindClose(handle);
		return true;
	}
}

