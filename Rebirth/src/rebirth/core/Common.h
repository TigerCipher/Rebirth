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
// File Name: Common.h
// Date File Created: 06/13/2022 at 3:03 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once

// Configuration
#define RB_ENABLE_PROFILER 0

// Macros

// Platform detection
#ifdef _WIN32
	#ifdef _WIN64
		#define RB_WINDOWS
	#else
		#error "x86 (32 bit) Builds are not supported!"
	#endif
	#elif defined(__APPLE__) || defined(__MACH__)
		#include <TargetConditionals.h>
		#if TARGET_IPHONE_SIMULATOR == 1
			#error "IOS simulator is not supported!"
		#elif TARGET_OS_IPHONE == 1
			#define RB_IOS
			#error "IOS is not supported!"
		#elif TARGET_OS_MAC == 1
			#define RB_MACOS
			#error "MacOS is not supported!"
		#else
			#error "Unknown Apple platform!"
	#endif
	#elif defined(__ANDROID__)
		#define HZ_PLATFORM_ANDROID
		#error "Android is not supported!"
	#elif defined(__linux__)
#define HZ_PLATFORM_LINUX
#error "Linux is not supported!"
	#else
		#error "Unknown platform!"
#endif


// DLL build check (might remove entirely)
#ifdef RB_WINDOWS
	#ifdef RB_DYNAMIC_LINK
		#ifdef RB_BUILD_DLL
			#define RB_API __declspec(dllexport)
		#else
			#define RB_API __declspec(dllimport)
		#endif
	#else
		#define RB_API
	#endif
#else
	#error Windows compatibility only (for now)
#endif

// Macro helpers
#define RB_EXPAND_MACRO(x) x
#define RB_STRINGIFY_MACRO(x) #x

// Enable asserts or not
#ifdef RB_DEBUG
	#define RB_ENABLE_ASSERTS
#endif

// Assert system - based on the one I wrote for Hazel - the project that inspired Rebirth
#ifdef RB_ENABLE_ASSERTS
	#include <filesystem> //#TODO: Move

	// TODO: #IMPORTANT: __debugbreak() is windows only I believe, need to make this cross platform
	#define RB_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { RB##type##FATAL(msg, __VA_ARGS__); RB##type##FATAL("Assertion check ({}) failed at {}:{}", RB_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__); __debugbreak(); /*RB_DEBUGBREAK();*/ } }
	#define RB_INTERNAL_ASSERT_WITH_FMT_MSG(type, check, msg, ...) RB_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: " msg, __VA_ARGS__)
	#define RB_INTERNAL_ASSERT_WITH_NOFMT_MSG(type, check, msg) RB_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {}", msg)
	#define RB_INTERNAL_ASSERT_WITH_NO_MSG(type, check) RB_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed!", "")

	#define RB_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, arg3, macro, ...) macro
	#define RB_INTERNAL_ASSERT_GET_MACRO(...) RB_EXPAND_MACRO( RB_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, RB_INTERNAL_ASSERT_WITH_FMT_MSG, RB_INTERNAL_ASSERT_WITH_NOFMT_MSG, RB_INTERNAL_ASSERT_WITH_NO_MSG) )

	// Accepts just the check, a check and a message, or a check, a formatted message, and arguments
	#define RB_ASSERT(...) RB_EXPAND_MACRO( RB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CLIENT_, __VA_ARGS__) )
	#define RB_CORE_ASSERT(...) RB_EXPAND_MACRO( RB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define RB_ASSERT(...)
	#define RB_CORE_ASSERT(...)
#endif




// Types and shortcuts

// encapsulating in namespace to avoid possible duplication
namespace rebirth
{
	using uint = unsigned int;
	using byte = unsigned char;
}

#define BIT(x) (1 << (x))

#define BIND_EVENT_FUNC(func) std::bind(&func, this, std::placeholders::_1)


// Smart Pointers (might move)

#include <memory>

template <typename T>
using UniquePtr = std::unique_ptr<T>;

// Scope as well as above in case I decide to make my own version of unique ptr
template <typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

// Ref as well as above in case I decide to make my own version of shared ptr
template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
constexpr Scope<T> createScope(Args&& ...args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
constexpr Ref<T> createRef(Args&& ...args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}