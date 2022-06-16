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

#define RB_EXPAND_MACRO(x) x
#define RB_STRINGIFY_MACRO(x) #x

#ifdef RB_ENABLE_ASSERTS
	#include <filesystem> //TODO: Move

	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define RB_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { RB##type##ERROR(msg, __VA_ARGS__); __debugbreak(); /*RB_DEBUGBREAK();*/ } }
	#define RB_INTERNAL_ASSERT_WITH_MSG(type, check, ...) RB_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define RB_INTERNAL_ASSERT_NO_MSG(type, check) RB_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", RB_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define RB_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define RB_INTERNAL_ASSERT_GET_MACRO(...) RB_EXPAND_MACRO( RB_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, RB_INTERNAL_ASSERT_WITH_MSG, RB_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define RB_ASSERT(...) RB_EXPAND_MACRO( RB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define RB_CORE_ASSERT(...) RB_EXPAND_MACRO( RB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define RB_ASSERT(...)
	#define RB_CORE_ASSERT(...)
#endif


// Types and shortcuts

using uint = unsigned int;

#define BIT(x) (1 << (x))


// Smart Pointers (might move)


//template<typename T>
//using list = std::vector<T>;

//temp
#include <memory>

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