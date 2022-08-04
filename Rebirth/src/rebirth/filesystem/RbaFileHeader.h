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
// File Name: RbaFileHeader.h
// Date File Created: 8/3/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "rebirth/core/Common.h"

namespace rebirth
{
	struct RbaHeader
	{
		char id[4] = { "RBA" };
		uint8 rbaVersion = 0;
		uint8 contentVersion = 0;
		char folderPath[100]{};
		char rbaName[50]{};
		uint8 numEntries = 0;
	};

	// #TODO: Might be a good idea to make this be the asset metadata?
	// Metadata would then store this, plus the UUID perhaps
	// Metadata for a file really shouldn't have a loaded flag, the asset itself should
	struct RbaFileEntry
	{
		char filepath[256]{};
		bool compressed = false;
		ulong uncompressedSize = 0;
		ulong compressedSize = 0;
		uint32 offset = 0;
	};
}

