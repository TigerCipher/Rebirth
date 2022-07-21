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
// File Name: EditorConsolePanel.h
// Date File Created: 7/17/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "EditorPanel.h"
#include "rebirth/debug/ConsoleMessage.h"

namespace rebirth
{

	class EditorConsolePanel : public EditorPanel
	{
	public:
		EditorConsolePanel();
		virtual ~EditorConsolePanel();
		void OnLoad();

		void OnImguiRender() override;

		void OnEvent(Event& e) override;

	private:
		void RenderMenu();
		void RenderConsole();
		static void PushMessage(const ConsoleMessage& msg);

	private:
		static constexpr uint32 sMessageBufferCapacity = 512;
		bool mShouldClearOnPlay = false;
		bool mCollapseMessages = false;
		bool mNewMessageAdded = false;
		bool mDisplayMessageInspector = false;
		bool mIsMessageInspectorHovered = false;

		std::array<ConsoleMessage, sMessageBufferCapacity> mMessageBuffer;
		uint32 mBufferBegin = 0;
		int32 mMessageFilters = LogLevel_Trace | LogLevel_Info | LogLevel_Warning
			| LogLevel_Error | LogLevel_Fatal;

		ConsoleMessage* mSelectedMessage = nullptr;

		friend class EditorConsoleSink;
	};
}

