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
// File Name: AssetManager.h
// Date File Created: 7/28/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "Asset.h"
#include "AssetRegistry.h"
#include "AssetLoader.h"
#include "rebirth/core/UUID.h"

namespace rebirth
{
	class AssetManager
	{
	public:
		AssetManager() = default;
		~AssetManager() = default;

		static void Init()
		{
			sRegistry.Clear();
		}
		static void Shutdown()
		{
			sLoadedAssets.clear();
		}
		static bool IsLoaded(UUID handle)
		{
			return sLoadedAssets.find(handle) != sLoadedAssets.end();
		}
		static UUID ImportAsset(const std::string& filepath);
		static UUID GetAssetHandle(const std::string& filepath);
		static void RemoveAsset(UUID handle);
		static AssetMetadata& GetMetadata(UUID handle);
		static AssetMetadata& GetMetadata(const std::filesystem::path& path);
		static std::filesystem::path ToRelative(const std::filesystem::path& path);

		static auto GetMetadataRegistry() { return sRegistry.GetRegistry(); }

		template<typename T>
		static Reference<T> GetAsset(const UUID handle)
		{
			AssetMetadata& meta = GetMetadata(handle);
			Reference<Asset> asset = nullptr;

			if(!meta.loaded)
			{
				meta.loaded = Load(meta, asset);
				if (!meta.loaded) return nullptr; // If the asset failed to load, return null

				asset->SetHandle(handle);
				asset->SetFlags(AssetFlag_Valid);
				sLoadedAssets[handle] = asset;
			}else
			{
				asset = sLoadedAssets[handle];
			}

			return asset.As<T>();
		}

		template<typename T>
		static void RegisterLoader(const AssetType type)
		{
			RB_CORE_INFO("Adding asset loader for {}", AssetTypeToString(type));
			sLoaders[type] = createScope<T>();
		}

	private:
		static bool Load(AssetMetadata& meta, Reference<Asset>& asset);
		static bool Save(Reference<Asset>& asset);

	private:
		static std::unordered_map<UUID, Reference<Asset>> sLoadedAssets;
		static std::unordered_map<AssetType, Scope<AssetLoader>> sLoaders;
		inline static AssetRegistry sRegistry;
		inline static AssetMetadata sNull;
	};
}
