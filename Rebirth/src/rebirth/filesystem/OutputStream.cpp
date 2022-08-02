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
// File Name: OutputStream.cpp
// Date File Created: 8/2/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"
#include "OutputStream.h"

namespace rebirth
{
	bool OutputStream::WriteString(const std::string& str)
	{
		return Write(str.data(), str.size() + 1) == str.size() + 1;
	}

	bool OutputStream::WriteSizedString(const std::string& str)
	{
		if (!WriteByte((byte)str.size()))
			return false;
		if (!Write(str.data(), str.size()))
			return false;
		return true;
	}

	bool OutputStream::WriteLine(const std::string& line)
	{
#ifdef RB_PLATFORM_WINDOWS
		return Write(line.c_str(), line.size()) && WriteByte((byte)'\r') && WriteByte((byte)'\n');
#else
		return Write(line.c_str(), line.size()) && WriteByte((byte)'\n');
#endif
	}
}

