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
// File Name: File.cpp
// Date File Created: 7/29/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "FileUtil.h"
#include "StringUtil.h"

namespace rebirth::file
{
	bool Exists(const fs::path& path)
	{
		return exists(path);
	}

	std::string GetFileName(const std::string& filePath)
	{
		auto it = std::find_if(filePath.rbegin(), filePath.rend(), [](const char c)
			{
				return IsSlash(c);
			});
		if (it == filePath.rend())
			return filePath;

		return filePath.substr(it.base() - filePath.begin());
	}

	std::string GetFileNameWithoutExtension(const std::string& filePath)
	{
		std::string fileName = GetFileName(filePath);
		const size_t dotPos = fileName.find('.');
		if (dotPos == std::string::npos)
			return fileName;
		return fileName.substr(0, dotPos);
	}

	std::string GetFileExtensionFromString(const std::string& filePath)
	{
		const size_t dotPos = filePath.rfind('.');
		if (dotPos == std::string::npos)
			return std::string();
		return filePath.substr(dotPos + 1);
	}

	std::string GetFileExtensionFromPath(const fs::path& path)
	{
		return path.string().substr(path.string().find_last_of('.') + 1);
	}

	std::string GetDirectoryPath(const std::string& filePath)
	{
		auto it = std::find_if(filePath.rbegin(), filePath.rend(), [](const char c)
			{
				return IsSlash(c);
			});
		if (it == filePath.rend())
		{
			if (filePath.rfind('.'))
				return "/";
			return filePath;
		}

		return filePath.substr(0, it.base() - filePath.begin());
	}

	void FixPath(std::string& path)
	{
		for (char& c : path)
		{
			if (c == '\\')
				c = '/';
		}
	}

	std::string FixPath(const std::string& path)
	{
		std::string output;
		if (path.substr(0, 2) == "./" || path.substr(0, 2) == ".\\")
			output = std::string(path.begin() + 2, path.end());
		else
		{
			output = path;
		}

		std::replace(output.begin(), output.end(), '\\', '/');
		ToLower(output);
		if (output.back() == '/')
			output.pop_back();
		return output;
	}

	std::string NormalizePath(const std::string& filepath)
	{
		auto output = std::string(filepath.begin(), filepath.end());
		NormalizeInline(output);
		return output;
	}

	void NormalizeInline(std::string& filepath)
	{
		for (char& c : filepath)
		{
			if (c == '\\')
				c = '/';
			else c = (char)tolower(c);
		}

		if(filepath.find("./") == 0)
		{
			filepath = std::string(filepath.begin() + 2, filepath.end());
		}
	}
}

