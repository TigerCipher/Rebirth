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
// File Name: AssetManager.cpp
// Date File Created: 7/28/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "AssetManager.h"
#include "rebirth/util/FileUtil.h"

namespace rebirth
{
	std::unordered_map<UUID, Reference<Asset>> AssetManager::sLoadedAssets;
	std::unordered_map<AssetType, Scope<AssetLoader>> AssetManager::sLoaders;


	UUID AssetManager::ImportAsset(const std::string& filepath)
	{
		if (filepath.empty()) return 0;

		// #TODO: Path should be relative to project, except when an engine/editor resource
		std::filesystem::path path = filepath;

		if (auto& meta = GetMetadata(path); meta.IsValid())
			return meta.handle;

		const std::string extension = file::GetFileExtensionFromPath(path);
		const AssetType type = AssetTypeFromExt(extension);

		if (type == AssetType_None)
			return 0;

		AssetMetadata data;
		data.handle = UUID();
		data.path = path;
		data.type = type;

		RB_CORE_INFO("Importing asset type [{}] from [{}] with UUID: {}", AssetTypeToString(type), filepath, data.handle);
		sRegistry[data.handle] = data;

		return data.handle;
	}

	UUID AssetManager::GetAssetHandle(const std::string& filepath)
	{
		for(auto& [handle, meta] : sRegistry)
		{
			if (meta.path == filepath)
				return handle;
		}
		return 0;
	}

	void AssetManager::RemoveAsset(UUID handle)
	{
		bool found = false;
		for(const auto& [path, meta] : sRegistry)
		{
			if(meta.handle == handle)
			{
				found = true;
				break;
			}
		}

		if (!found) return;

		const AssetMetadata& meta = GetMetadata(handle);
		sRegistry.Remove(meta.handle);

		if (sLoadedAssets.find(handle) != sLoadedAssets.end())
			sLoadedAssets.erase(handle);
	}

	AssetMetadata& AssetManager::GetMetadata(UUID handle)
	{
		if (sRegistry.Contains(handle))
			return sRegistry[handle];
		return sNull;
	}

	AssetMetadata& AssetManager::GetMetadata(const std::filesystem::path& path)
	{
		for(auto& [handle, meta] : sRegistry)
		{
			if (meta.path == path)
				return meta;
		}

		return sNull;
	}

	std::filesystem::path AssetManager::ToRelative(const std::filesystem::path& path)
	{
		return path; // #TODO: relative to project or engine/editor
	}

	bool AssetManager::Load(AssetMetadata& meta, Reference<Asset>& asset)
	{
		if (sLoaders.find(meta.type) == sLoaders.end())
		{
			RB_CORE_ERROR("No loaders for {} : {}", AssetTypeToString(meta.type), meta.path.string());
			return false;
		}

		return sLoaders[meta.type]->Load(meta, asset);
	}

	bool AssetManager::Save(Reference<Asset>& asset)
	{
		if (!asset) return false;
		const AssetMetadata& meta = GetMetadata(asset->GetHandle());

		if (sLoaders.find(meta.type) == sLoaders.end())
		{
			RB_CORE_ERROR("No loaders for {} : {}", AssetTypeToString(meta.type), meta.path.string());
			return false;
		}

		return sLoaders[meta.type]->Save(asset);
	}

}

