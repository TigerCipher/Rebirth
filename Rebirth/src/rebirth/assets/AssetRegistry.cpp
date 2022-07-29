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

	bool AssetRegistry::Contains(const UUID handle) const
	{
		return mRegistry.find(handle) != mRegistry.end();
	}

	const AssetMetadata& AssetRegistry::Get(const UUID handle) const
	{
		return mRegistry.at(handle);
	}

	size_t AssetRegistry::Remove(const UUID handle)
	{
		return mRegistry.erase(handle);
	}

	AssetMetadata& AssetRegistry::Get(const UUID handle)
	{
		return mRegistry.at(handle);
	}

	AssetMetadata& AssetRegistry::operator[](const UUID handle)
	{
		return mRegistry[handle];
	}
}

