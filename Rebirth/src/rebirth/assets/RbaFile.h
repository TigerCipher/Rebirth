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
// File Name: AssetPackage.h
// Date File Created: 7/30/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#define RBA_NEWEST_VERSION 1

namespace rebirth
{
	class RbaFile
	{
	public:
		struct RbaHeader
		{
			char id[4] = { "RBA" };
			uint8 rbaVersion = 0;
			uint8 contentVersion = 0;
			char folderPath[100]{};
			char rbaName[50]{};
			uint8 numEntries = 0;
		};

		struct RbaFileEntry
		{
			char filepath[256]{};
			bool compressed = false;
			uint32 uncompressedSize = 0;
			uint32 compressedSize = 0;
			uint32 offset = 0;
		};

	public:

		RbaFile(const std::string& filepath, const uint32 rbaFileVersion = RBA_NEWEST_VERSION, const uint32 minCompressionSize = 524288) :
			mFilepath(filepath), mRbaFileVersion(rbaFileVersion), mMinCompressionFileSize(minCompressionSize) {}

		bool CreateRbaFile(const std::string& srcDir, std::string& virtualDir,
			uint8 contentVersion, bool compress, uint32 minCompressBias, bool hq = true);

		bool ExtractRbaFile(const std::string& destDir);

	private:

		bool Compress(void* inData, size_t inDataSize, bool hc, std::vector<char>& outData);
		bool Decompress(const void* inData, size_t compressedSize, size_t uncompressedSize, std::vector<char>& outData);

		std::string mFilepath;
		uint32 mMinCompressionFileSize = 524288;
		uint32 mRbaFileVersion = RBA_NEWEST_VERSION;
		
	};
}

