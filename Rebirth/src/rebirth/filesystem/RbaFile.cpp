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
// File Name: AssetPackage.cpp
// Date File Created: 7/30/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "RbaFile.h"

#include "rebirth/util/FileUtil.h"

#include <zlib.h>

#define ENABLE_COMPRESSION 1

namespace rebirth
{
	bool RbaFile::CreateRbaFile(const std::string& srcDir, std::string& virtualDir,
		uint8 contentVersion, bool compress, uint32 minCompressBias)
	{
		std::string targetFile = mFilepath;
		RbaHeader header;
		std::string pakName = file::GetFileName(targetFile);
		std::string fileDir = file::GetDirectoryPath(targetFile);

		memcpy(header.rbaName, pakName.data(), pakName.length() + 1);
		memcpy(header.folderPath, fileDir.data(), fileDir.length() + 1);

		file::FixPath(virtualDir);

		header.numEntries = 0;
		header.rbaVersion = mRbaFileVersion;
		header.contentVersion = contentVersion;

		std::vector<char> dataBuffer;
		std::vector<RbaFileEntry> fileEntries;

		uint32 compressBias = minCompressBias > 0 ? minCompressBias : mMinCompressionFileSize;

		for (auto& dirEntry : std::filesystem::recursive_directory_iterator(srcDir))
		{
			const auto& path = dirEntry.path();
			if (dirEntry.is_directory()) continue;

			std::string filename = path.string();

			for (char& c : filename)
				c = (char)tolower(c);

			std::ifstream filestream(filename, std::ios::ate | std::ios::binary);
			if (filestream.fail())
			{
				RB_CORE_WARN("File failed to load {}", filename);
				continue;
			}
			RbaFileEntry rbaFileEntry;
			memset(&rbaFileEntry, 0, sizeof(RbaFileEntry));
			rbaFileEntry.uncompressedSize = (uint32)filestream.tellg();
			std::string fname = filename.substr(virtualDir.size());
			RB_CORE_INFO("[PACKING] [{}] to [{}] in [{}]", filename, fname, targetFile);
			memcpy(rbaFileEntry.filepath, fname.data(), fname.length() + 1);
			rbaFileEntry.offset = (uint32)dataBuffer.size();
			rbaFileEntry.compressed = rbaFileEntry.uncompressedSize > compressBias && compress == true;
			filestream.seekg(0);

			std::vector<byte> fileData;
			fileData.resize(rbaFileEntry.uncompressedSize);
			filestream.read((char*)fileData.data(), rbaFileEntry.uncompressedSize);
#if ENABLE_COMPRESSION
			if (rbaFileEntry.compressed)
			{
				std::vector<Bytef> compressedData;
				if (bool res = Compress(fileData.data(), (uLong)fileData.size(), compressedData, &rbaFileEntry.compressedSize); !res) return res;
				dataBuffer.insert(dataBuffer.end(), compressedData.data(), compressedData.data() + compressedData.size());
				RB_CORE_TRACE("Compressed File [{}]. Uncompressed Size = {}, Compressed Size = {}", rbaFileEntry.filepath, rbaFileEntry.uncompressedSize, rbaFileEntry.compressedSize);
			}
			else
#endif
			{
				rbaFileEntry.compressedSize = rbaFileEntry.uncompressedSize;
				dataBuffer.insert(dataBuffer.end(), fileData.begin(), fileData.end());
			}

			fileEntries.push_back(rbaFileEntry);
			filestream.close();
		}

		header.numEntries = (uint8)fileEntries.size();

		std::ofstream output(targetFile, std::ios::binary);

		output.write(reinterpret_cast<char*>(&header), sizeof(RbaHeader));

		const uint32 baseOffset = sizeof(RbaHeader) + (uint32)fileEntries.size() * sizeof(RbaFileEntry);

		for (RbaFileEntry& e : fileEntries)
		{
			e.offset += baseOffset;
			output.write(reinterpret_cast<char*>(&e), sizeof(RbaFileEntry));
		}

		output.write(dataBuffer.data(), dataBuffer.size());

		output.close();
		return true;
	}

	bool RbaFile::ExtractRbaFile(const std::string& destDir)
	{
		std::string rbaFile = mFilepath;
		std::ifstream rbaFileStream(rbaFile, std::ios::binary);
		if (rbaFileStream.fail())
		{
			RB_CORE_ERROR("Failed to open RBA file {}", rbaFile);
			return false;
		}
		RbaHeader header;
		rbaFileStream.read(reinterpret_cast<char*>(&header), sizeof(RbaHeader));
		if (std::string(header.id) != "RBA")
		{
			RB_CORE_ERROR("RBA file {} is of bad format", rbaFile);
			return false;
		}
		std::vector<RbaFileEntry> pEntries(header.numEntries);
		//auto* pEntries = new RbaFileEntry[header.numEntries];
		rbaFileStream.read(reinterpret_cast<char*>(pEntries.data()), header.numEntries * sizeof(RbaFileEntry));

		for (int i = 0; i < header.numEntries; i++)
		{
			RbaFileEntry& entry = pEntries[i];
			//rbaFileStream.seekg(entry.Offset);
			std::vector<byte> buffer(entry.uncompressedSize);
#if ENABLE_COMPRESSION
			if (entry.compressed)
			{
				std::vector<byte> compressedBuffer(entry.compressedSize);
				rbaFileStream.read((char*)compressedBuffer.data(), compressedBuffer.size());
				if (!Decompress(compressedBuffer.data(), (uLong)compressedBuffer.size(), &entry.uncompressedSize, buffer))
				{
					RB_CORE_ERROR("Failed to decompress file {}. Uncompressed Size: {} -- {}, Compressed Size: {}",
						entry.filepath, entry.uncompressedSize, buffer.size(), entry.compressedSize);
					//delete[] pEntries;
					return false;
				}
				RB_CORE_TRACE("Decompressed [{}] (Compressed Size: {}, Uncompressed Size: {})", entry.filepath, entry.compressedSize, entry.uncompressedSize);
			}
			else
#endif
			{
				buffer.resize(entry.uncompressedSize);
				rbaFileStream.read((char*)buffer.data(), buffer.size());
			}
			std::filesystem::create_directories(destDir + file::GetDirectoryPath(entry.filepath));
			std::ofstream outputStream(destDir + entry.filepath, std::ios::binary);
			if (outputStream.fail())
			{
				RB_CORE_ERROR("Failed to create file {}", destDir + entry.filepath);
				//delete[] pEntries;
				return false;
			}

			RB_CORE_INFO("Extracting [{}] to [{}]", entry.filepath, destDir + entry.filepath);
			outputStream.write((char*)buffer.data(), buffer.size());
		}
		//delete[] pEntries;
		return true;
	}


	bool RbaFile::Compress(const byte* srcData, const ulong srcDataSize, std::vector<byte>& destData, ulong* compressedSize)
	{
		*compressedSize = compressBound(srcDataSize);
		destData.resize(*compressedSize);
		const int ret = compress(destData.data(), compressedSize, srcData, srcDataSize);
		//Z_MEM_ERROR = -4
		//Z_BUF_ERROR = -5
		//Z_DATA_ERROR = -3
		return ret == Z_OK;
	}

	bool RbaFile::Decompress(const byte* srcData, const ulong compressedSize, ulong* uncompressedSize,
		std::vector<byte>& destData)
	{
		const int ret = uncompress(destData.data(), uncompressedSize, srcData, compressedSize);
		return ret == Z_OK;
	}
}
