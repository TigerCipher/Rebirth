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
// File Name: AssetTypes.cpp
// Date File Created: 7/27/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "AssetTypes.h"

namespace rebirth
{

	AssetType AssetTypeFromString(const std::string& type)
	{
		if (type == "None") return AssetType_None;
		if (type == "Texture") return AssetType_Texture;
		if (type == "Atlas") return AssetType_Atlas;
		if (type == "PhysicsMaterial") return AssetType_PhysicsMaterial;
		if (type == "Audio") return AssetType_Audio;
		if (type == "Script") return AssetType_Script;
		if (type == "Light") return AssetType_Light;
		if (type == "Scene") return AssetType_Scene;

		return AssetType_None;
	}

	const char* AssetTypeToString(AssetType type)
	{
		switch (type)
		{
			case AssetType_None: return "None";
			case AssetType_Texture: return "Texture";
			case AssetType_Atlas: return "Atlas";
			case AssetType_PhysicsMaterial: return "PhysicsMaterial";
			case AssetType_Audio: return "Audio";
			case AssetType_Light: return "Light";
			case AssetType_Script: return "Script";
			case AssetType_Scene: return "Scene";
			default: return "None";
		}
	}

	AssetType AssetTypeFromExt(const std::string& ext)
	{
		if (ext == "png") return AssetType_Texture;
		if (ext == "jpg") return AssetType_Texture;
		if (ext == "jpeg") return AssetType_Texture;
		if (ext == "rebirth") return AssetType_Scene;
		if (ext == "cs") return AssetType_Script;
		if (ext == "atlas") return AssetType_Atlas;

		return AssetType_None;
	}
}

