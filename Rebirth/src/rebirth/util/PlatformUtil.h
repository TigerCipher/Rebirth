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
// File Name: PlatformUtil.h
// Date File Created: 6/30/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once

#include <string>

namespace rebirth
{
	class FileDialog
	{
	public:
		static std::string OpenFile(const char* filters);
		static std::string SaveFile(const char* filters);
	};

	class Time
	{
	public:
		static void Init();
		static uint64 GetTimerValue();
		static uint64 GetTimerFrequency();
		static double GetTime();
	};

}
