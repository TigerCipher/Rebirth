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
// File Name: ConsoleMessage.h
// Date File Created: 7/17/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "rebirth/core/Common.h"
#include <string>

namespace rebirth
{
	enum LogLevel : int16
	{
		LogLevel_None = -1,
		LogLevel_Trace = BIT(0),
		LogLevel_Info = BIT(1),
		LogLevel_Warning = BIT(2),
		LogLevel_Error = BIT(3),
		LogLevel_Fatal = BIT(4),
	};

	class ConsoleMessage
	{
	public:

		ConsoleMessage() = default;
		ConsoleMessage(const std::string& msg, LogLevel category) :
			mMessageId(std::hash<std::string>()(msg)), mMessage(msg), mCount(1), mLogLevel(category) {}

		uint64 GetId() const { return mMessageId; }
		const std::string& GetMessage() const { return mMessage; }
		uint32 GetCount() const { return mCount; }
		LogLevel GetLogLevel() const { return mLogLevel; }

	private:
		uint64 mMessageId = 0;
		std::string mMessage = "";
		uint32 mCount = 0;
		LogLevel mLogLevel = LogLevel_None;

		// Editor
		friend class EditorConsolePanel;
	};
}

