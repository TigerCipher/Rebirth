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
// File Name: MathUtil.h
// Date File Created: 6/30/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include <glm/glm.hpp>

namespace rebirth::math
{
	bool Decompose(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);

	template<typename T>
	constexpr T Clamp(const T value, const T high, const T low)
	{
		if (value > high) return high;
		if (value < low) return low;
		return value;
	}

	std::string ToBase(uint32 number, uint32 base);

	inline std::string ToBinary(const uint32 number)
	{
		return ToBase(number, 2);
	}

	inline std::string ToHex(const uint32 number)
	{
		return ToBase(number, 16);
	}
}

