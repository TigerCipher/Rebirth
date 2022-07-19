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
// File Name: ApplicationDesc.h
// Date File Created: 7/19/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once
#include "Common.h"
#include <string>

namespace rebirth
{
	enum WindowFlags : int16
	{
		WindowFlag_None = 0,
		WindowFlag_NotDecorated = BIT(0),
		WindowFlag_Maximized = BIT(1),
		WindowFlag_Fullscreen = BIT(2),
		WindowFlag_VSync = BIT(3)
	};

	struct ApplicationDesc
	{
		std::string title;
		uint32 windowWidth = 1920;
		uint32 windowHeight = 1080;
		int32 flags = WindowFlag_None;
	};
}

