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
// File Name: Asset.h
// Date File Created: 7/27/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "AssetTypes.h"
#include "rebirth/core/Common.h"
#include "rebirth/memory/Reference.h"
#include "rebirth/core/UUID.h"

namespace rebirth
{
	enum AssetFlag : uint16
	{
		AssetFlag_None = BIT(0),
		AssetFlag_Valid = BIT(1),
		AssetFlag_Missing = BIT(2),
		AssetFlag_Invalid = BIT(3)
	};

	struct AssetMetadata
	{
		UUID handle;
		AssetType type = AssetType_None;

		std::filesystem::path path = "";
		bool loaded = false;

		bool IsValid() { return handle && type != AssetType_None; }
	};

	class Asset : public RefCount
	{
		friend class AssetManager;
	public:

		virtual ~Asset() = default;

		const AssetType GetType() const { return mType; }
		UUID GetHandle() { return mHandle; }
		AssetFlag GetFlags() { return mFlags; }

		bool IsValid() { return mHandle != 0; }

	protected:
		UUID mHandle = 0;
		AssetType mType;
		AssetFlag mFlags = AssetFlag_None;
	private:
		void SetHandle(UUID handle) { mHandle = handle; }
		void SetFlags(AssetFlag flags) { mFlags = flags; }
	};
}
