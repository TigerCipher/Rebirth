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
// File Name: Log.h
// Date File Created: 06/13/2022 at 11:39 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once

#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace rebirth
{
	class RB_API Log
	{
	public:
		static void Init();

		static SharedPtr<spdlog::logger>& GetCoreLogger() { return sCoreLogger; }
		static SharedPtr<spdlog::logger>& GetClientLogger() { return sClientLogger; }

	private:
		static SharedPtr<spdlog::logger> sCoreLogger;
		static SharedPtr<spdlog::logger> sClientLogger;
	};
}

#define RB_CORE_TRACE(...)	::rebirth::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RB_CORE_INFO(...)	::rebirth::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RB_CORE_WARN(...)	::rebirth::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RB_CORE_ERROR(...)	::rebirth::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RB_CORE_FATAL(...)	::rebirth::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define RB_CLIENT_TRACE(...)	::rebirth::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RB_CLIENT_INFO(...)		::rebirth::Log::GetClientLogger()->info(__VA_ARGS__)
#define RB_CLIENT_WARN(...)		::rebirth::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RB_CLIENT_ERROR(...)	::rebirth::Log::GetClientLogger()->error(__VA_ARGS__)
#define RB_CLIENT_FATAL(...)	::rebirth::Log::GetClientLogger()->fatal(__VA_ARGS__)


// Not ideal since still might use in engine headers, which become client when built on sandbox
//#ifdef RB_BUILD_DLL
//	#define RB_TRACE(...)	RB_CORE_TRACE(__VA_ARGS__)
//	#define RB_INFO(...)	RB_CORE_INFO(__VA_ARGS__)
//	#define RB_WARN(...)	RB_CORE_WARN(__VA_ARGS__)
//	#define RB_ERROR(...)	RB_CORE_ERROR(__VA_ARGS__)
//	#define RB_FATAL(...)	RB_CORE_FATAL(__VA_ARGS__)
//#else
//	#define RB_TRACE(...)	RB_CLIENT_TRACE(__VA_ARGS__)
//	#define RB_INFO(...)	RB_CLIENT_INFO(__VA_ARGS__)
//	#define RB_WARN(...)	RB_CLIENT_WARN(__VA_ARGS__)
//	#define RB_ERROR(...)	RB_CLIENT_ERROR(__VA_ARGS__)
//	#define RB_FATAL(...)	RB_CLIENT_FATAL(__VA_ARGS__)
//#endif
