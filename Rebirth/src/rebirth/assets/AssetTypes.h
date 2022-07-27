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
// File Name: AssetTypes.h
// Date File Created: 7/27/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

namespace rebirth
{
	enum AssetType
	{
		AssetType_None = 0,
		AssetType_Texture,

		// These are planned future types
		AssetType_Atlas,
		AssetType_PhysicsMaterial,
		AssetType_Audio,
		AssetType_Light,
		AssetType_Script
	};

	AssetType AssetTypeFromString(const std::string& type);
	const char* AssetTypeToString(AssetType type);
}

