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
// File Name: Log.cpp
// Date File Created: 06/13/2022 at 11:39 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

SharedPtr<spdlog::logger> rebirth::Log::sCoreLogger;
SharedPtr<spdlog::logger> rebirth::Log::sClientLogger;


void rebirth::Log::Init()
{
	spdlog::set_pattern("%^[%T] [%n] [%l] %v%$");

	sCoreLogger = spdlog::stdout_color_mt("Rebirth Engine");
	sCoreLogger->set_level(spdlog::level::trace);
	
	sClientLogger = spdlog::stdout_color_mt("Client");
	sClientLogger->set_level(spdlog::level::trace);
}
