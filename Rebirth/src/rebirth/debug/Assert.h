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
// File Name: Assert.h
// Date File Created: 6/27/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once
#include "platform/PlatformDetection.h"

#include <filesystem>

#ifdef RB_DEBUG
#	ifdef RB_PLATFORM_WINDOWS
#		define RB_DEBUG_BREAK() __debugbreak()
#	elif defined(RB_PLATFORM_LINUX)
#		include <signal.h>
#		define RB_DEBUG_BREAK() raise(SIGTRAP)
#	else
#		error Platform doesn't support debug breaking in this engine
#	endif
#	define RB_ENABLE_ASSERTS
#else
#define RB_DEBUG_BREAK()
#endif




// Assert system - based on the one I wrote for Hazel - the project that inspired Rebirth
#ifdef RB_ENABLE_ASSERTS
#	define RB_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { RB##type##FATAL(msg, __VA_ARGS__); RB##type##FATAL("Assertion check ({}) failed at {}:{}", RB_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__); RB_DEBUG_BREAK(); } }
#	define RB_INTERNAL_ASSERT_WITH_FMT_MSG(type, check, msg, ...) RB_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: " msg, __VA_ARGS__)
#	define RB_INTERNAL_ASSERT_WITH_NOFMT_MSG(type, check, msg) RB_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {}", msg)
#	define RB_INTERNAL_ASSERT_WITH_NO_MSG(type, check) RB_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed!", "")

#	define RB_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, arg3, macro, ...) macro
#	define RB_INTERNAL_ASSERT_GET_MACRO(...) RB_EXPAND_MACRO( RB_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, RB_INTERNAL_ASSERT_WITH_FMT_MSG, RB_INTERNAL_ASSERT_WITH_NOFMT_MSG, RB_INTERNAL_ASSERT_WITH_NO_MSG) )

	// Accepts just the check, a check and a message, or a check, a formatted message, and arguments
#	define RB_ASSERT(...) RB_EXPAND_MACRO( RB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CLIENT_, __VA_ARGS__) )
#	define RB_CORE_ASSERT(...) RB_EXPAND_MACRO( RB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#	define RB_ASSERT(...)
#	define RB_CORE_ASSERT(...)
#endif

