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
// File Name: PlatformDetection.h
// Date File Created: 6/27/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once

#ifdef _WIN32
#	ifdef _WIN64
#		define RB_PLATFORM_WINDOWS
#	else
#		error "x86 (32 bit) Builds are not supported!"
#	endif
#elif defined(__APPLE__) || defined(__MACH__)
#	include <TargetConditionals.h>
#	if TARGET_IPHONE_SIMULATOR == 1
#		error "IOS simulator is not supported!"
#	elif TARGET_OS_IPHONE == 1
#		define RB_IOS
#		error "IOS is not supported!"
#	elif TARGET_OS_MAC == 1
#		define RB_MACOS
#		error "MacOS is not supported!"
#	else
#		error "Unknown Apple platform!"
#	endif
#elif defined(__ANDROID__)
#	define RB_PLATFORM_ANDROID
#	error "Android is not supported!"
#elif defined(__linux__)
#	define RB_PLATFORM_LINUX
#	error "Linux is not supported!"
#else
#	error "Unknown platform!"
#endif