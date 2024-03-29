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
// File Name: Colors.h
// Date File Created: 7/17/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include <glm/glm.hpp>

namespace rebirth
{
	using Color = glm::vec4;
	class Colors
	{
	public:
		static constexpr Color RED		= { 1, 0, 0, 1 };
		static constexpr Color GREEN	= { 0, 1, 0, 1 };
		static constexpr Color BLUE		= { 0, 0, 1, 1 };
		static constexpr Color YELLOW	= { 1, 1, 0, 1 };
		static constexpr Color CYAN		= { 0, 1, 1, 1 };
		static constexpr Color MAGENTA	= { 1, 0, 1, 1 };
	};
}

