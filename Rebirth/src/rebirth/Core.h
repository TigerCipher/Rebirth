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
// File Name: Core.h
// Date File Created: 06/13/2022 at 3:03 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once


// Macros

#ifdef RB_WINDOWS
	#ifdef RB_BUILD_DLL
		#define RB_API __declspec(dllexport)
	#else
		#define RB_API __declspec(dllimport)
	#endif
#else
	#error Windows compatibility only (for now)
#endif


// Types and shortcuts

using uint = unsigned int;

#define BIT(x) (1 << x)


// Smart Pointers (might move)


template<typename T>
using list = std::vector<T>;

template <typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T, typename... Args>
constexpr UniquePtr<T> createScope(Args&& ...args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
constexpr SharedPtr<T> createRef(Args&& ...args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}