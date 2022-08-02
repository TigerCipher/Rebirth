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
// File Name: MountPoint.h
// Date File Created: 8/2/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once
namespace rebirth
{

	class File;

	class MountPoint
	{
	public:
		MountPoint(const std::string& physicalPath, const int order = INT_MAX) :
		mPhysicalPath(physicalPath), mOrder(order) {}

		virtual bool OnMount() = 0;
		virtual Scope<File> GetFile(const std::string& filePath) = 0;
		virtual bool HasFile(const std::string& filePath) const = 0;

		const std::string& GetPhysicalPath() const { return mPhysicalPath; }
		int GetOrder() const { return mOrder; }

	protected:
		std::string mPhysicalPath;
		int mOrder;
	};
}

