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
// File Name: OutputStream.h
// Date File Created: 8/2/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#define RB_OUTPUTSTREAM_WRITE(name, type) bool Write##name(const type value) { return Write(&value, sizeof(type)) == sizeof(type); } 

namespace rebirth
{
	class OutputStream
	{
	public:
		OutputStream() = default;
		virtual ~OutputStream() = default;

		virtual size_t Write(const void* data, size_t size) = 0;

		bool WriteString(const std::string& str);
		bool WriteSizedString(const std::string& str);
		bool WriteLine(const std::string& line);

		RB_OUTPUTSTREAM_WRITE(Int, int)
		RB_OUTPUTSTREAM_WRITE(Int64, int64)
		RB_OUTPUTSTREAM_WRITE(Float, float)
		RB_OUTPUTSTREAM_WRITE(Double, double)
		RB_OUTPUTSTREAM_WRITE(Char, char)
		RB_OUTPUTSTREAM_WRITE(Byte, byte)
	};
}

