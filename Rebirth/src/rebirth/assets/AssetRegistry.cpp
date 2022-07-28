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
// File Name: AssetRegistry.cpp
// Date File Created: 7/28/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "AssetRegistry.h"

namespace rebirth
{

	static std::filesystem::path GetKey(const std::filesystem::path& path)
	{
		auto key = path;
		if (key.empty())
			key = path.lexically_normal();
		return key;
	}

	bool AssetRegistry::Contains(const std::filesystem::path& path) const
	{
		const auto key = GetKey(path);
		return mRegistry.find(key) != mRegistry.end();
	}

	const AssetMetadata& AssetRegistry::Get(const std::filesystem::path& path) const
	{
		const auto key = GetKey(path);
		return mRegistry.at(key);
	}

	size_t AssetRegistry::Remove(const std::filesystem::path& path)
	{
		const auto key = GetKey(path);
		return mRegistry.erase(key);
	}

	AssetMetadata& AssetRegistry::Get(const std::filesystem::path& path)
	{
		const auto key = GetKey(path);
		return mRegistry.at(key);
	}

	AssetMetadata& AssetRegistry::operator[](const std::filesystem::path& path)
	{
		RB_CORE_ASSERT(!path.string().empty(), "Filepath cannot be empty");
		const auto key = GetKey(path);
		return mRegistry[key];
	}
}

