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
// File Name: StringUtil.cpp
// Date File Created: 7/21/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "StringUtil.h"

namespace rebirth
{

	std::string ReplaceAll(const std::string& orig, const std::string& find, const std::string& replace)
	{
		std::string result;
		size_t find_len = find.size();
		size_t pos, from = 0;
		while (std::string::npos != (pos = orig.find(find, from)))
		{
			result.append(orig, from, pos - from);
			result.append(replace);
			from = pos + find_len;
		}

		result.append(orig, from, std::string::npos);
		return result;
	}

}

