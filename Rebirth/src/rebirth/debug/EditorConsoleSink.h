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
// File Name: EditorConsoleSink.h
// Date File Created: 7/17/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "rebirth/core/Common.h"
#include "ConsoleMessage.h"
#include "rebirth/imgui/EditorConsolePanel.h"

#pragma warning(push, 0)
#include <spdlog/sinks/base_sink.h>
#pragma warning(pop)

#include <mutex>

namespace rebirth
{
	class EditorConsoleSink : public spdlog::sinks::base_sink<std::mutex>
	{
	public:
		EditorConsoleSink(uint32 bufferCapacity) :
			mMessageBufferCapacity(bufferCapacity), mMessageBuffer(bufferCapacity) {}
		virtual ~EditorConsoleSink() = default;

		EditorConsoleSink(const EditorConsoleSink&) = delete;
		EditorConsoleSink& operator=(const EditorConsoleSink&) = delete;

	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override
		{
			spdlog::memory_buf_t formatted;
			spdlog::sinks::base_sink<std::mutex>::formatter_->format(msg, formatted);
			mMessageBuffer[mMessageCount++] = ConsoleMessage(fmt::to_string(formatted), GetCategory(msg.level));

			if (mMessageCount == mMessageBufferCapacity)
				flush_();
		}

		void flush_() override
		{
			for (const auto& msg : mMessageBuffer)
			{
				if (msg.GetLogLevel() == LogLevel_None)
					continue;
				EditorConsolePanel::PushMessage(msg);
			}

			mMessageCount = 0;
		}
	private:
		static LogLevel GetCategory(spdlog::level::level_enum level)
		{
			switch (level)
			{
				case spdlog::level::trace:
				case spdlog::level::debug:
					return LogLevel_Trace;
				case spdlog::level::info:
					return LogLevel_Info;
				case spdlog::level::warn:
					return LogLevel_Warning;
				case spdlog::level::err:
					return LogLevel_Error;
				case spdlog::level::critical:
					return LogLevel_Fatal;
				default:
					RB_CORE_ASSERT(false, "Invalid Console Message Category");
					return LogLevel_None;
			}
		}
	private:
		uint32 mMessageBufferCapacity;
		std::vector<ConsoleMessage> mMessageBuffer;
		uint32 mMessageCount = 0;
	};
}

