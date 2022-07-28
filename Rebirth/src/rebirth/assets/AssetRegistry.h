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
// File Name: AssetRegistry.h
// Date File Created: 7/28/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "Asset.h"


namespace rebirth
{
	class AssetRegistry
	{
	public:
		AssetRegistry() = default;
		~AssetRegistry() = default;

		bool Contains(const std::filesystem::path& path) const;

		AssetMetadata& Get(const std::filesystem::path& path);
		const AssetMetadata& Get(const std::filesystem::path& path) const;
		AssetMetadata& operator[](const std::filesystem::path& path);

		size_t Remove(const std::filesystem::path& path);

		void Clear() { mRegistry.clear(); }

		auto GetRegistry() const { return mRegistry; }

		auto begin() { return mRegistry.begin(); }
		auto end() { return mRegistry.end(); }
		auto cbegin() const { return mRegistry.cbegin(); }
		auto cend() const { return mRegistry.cend(); }
	private:
		std::unordered_map<std::filesystem::path, AssetMetadata> mRegistry;
	};
}

