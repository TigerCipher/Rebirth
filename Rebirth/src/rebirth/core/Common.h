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

#include "platform/PlatformDetection.h"
#include "rebirth/debug/Assert.h"

// Configuration
#define RB_ENABLE_PROFILER 0

// Macro helpers
#define RB_EXPAND_MACRO(x) x
#define RB_STRINGIFY_MACRO(x) #x
#define BIT(x) (1 << (x))
//#define BIND_EVENT_FUNC(func) std::bind(&func, this, std::placeholders::_1)
#define BIND_EVENT_FUNC(func) [this](auto&&... args)-> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }
#define RB_DELETE(x) delete x; x = nullptr;


// Types and shortcuts

// encapsulating in namespace to avoid possible duplication
namespace rebirth
{
	// ensure x bit, but I hate using _t, it looks ugly
	using uint8 = uint8_t;
	using uint16 = uint16_t;
	using uint32 = uint32_t;
	using uint64 = uint64_t;
	using byte = unsigned char;
}

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

