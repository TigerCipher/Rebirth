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
// File Name: UUID.cpp
// Date File Created: 7/15/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "UUID.h"

#include <random>

namespace rebirth
{

	static std::random_device sRandDevice;
	static std::mt19937_64 sRandEngine(sRandDevice());
	static std::uniform_int_distribution<uint64> sDistribution;


	UUID::UUID() :
		mUUID(sDistribution(sRandEngine))
	{

	}

	UUID::UUID(uint64 id) :
		mUUID(id)
	{

	}

}


