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
// File Name: File.h
// Date File Created: 7/29/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

namespace fs = std::filesystem;

namespace rebirth::file {

	inline bool IsSlash(const char c)
	{
		return c == '\\' || c == '/';
	}

	bool Exists(const fs::path& path);

	std::string GetFileName(const std::string& filePath);
	std::string GetFileNameWithoutExtension(const std::string& filePath);
	std::string GetFileExtensionFromString(const std::string& filePath);
	std::string GetFileExtensionFromPath(const fs::path& path);
	std::string GetDirectoryPath(const std::string& filePath);
	void FixPath(std::string& path);
	std::string FixPath(const std::string& path);

	std::string NormalizePath(const std::string& filepath);
	void NormalizeInline(std::string& filepath);
}
