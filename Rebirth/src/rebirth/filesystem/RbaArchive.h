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


namespace rebirth
{
	class RbaArchive
	{
	public:

		RbaArchive(const std::string& filepath, const uint8 rbaFileVersion = RBA_NEWEST_VERSION) :
			mFilepath(filepath), mRbaFileVersion(rbaFileVersion) {}

		bool CreateRbaFile(const std::string& srcDir, std::string& virtualDir,
			uint8 contentVersion, bool compress = true, uint32 minCompressBias = 0);

		bool ExtractRbaFile(const std::string& destDir);

	private:

		bool Compress(const byte* srcData, ulong srcDataSize, std::vector<byte>& destData, ulong* compressedSize);
		bool Decompress(const byte* srcData, ulong compressedSize, ulong* uncompressedSize, std::vector<byte>& destData);

		std::string mFilepath;
		uint32 mMinCompressionFileSize = 524288;
		uint8 mRbaFileVersion = RBA_NEWEST_VERSION;
		
	};
}

