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
// File Name: StringUtil.h
// Date File Created: 7/21/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

namespace rebirth
{
	std::string ReplaceAll(const std::string& orig, const std::string& find, const std::string& replace);

	inline std::string ToLower(const std::string& str)
	{
		std::string output = str;
		for (char& c : output)
			c = (char)tolower(c);
		return output;
	}

	inline void ToLower(char* str)
	{
		while(*str != '\0')
		{
			*str = (char)tolower(*str);
			++str;
		}
	}

	inline void ToLower(std::string& str)
	{
		for(char& c : str)
		{
			c = (char)tolower(c);
		}
	}
	
}

