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
// File Name: EditorConsolePanel.cpp
// Date File Created: 7/17/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "EditorConsolePanel.h"

#include "rebirth/renderer/Texture.h"
#include "rebirth/events/SceneEvent.h"

#include <imgui.h>
#include <imgui_internal.h>



namespace rebirth
{
	static EditorConsolePanel* sInstance = nullptr;

	static ImVec4 sTraceButtonOnTint = ImVec4(0.431372549f, 0.431372549f, 0.431372549f, 1.0f);
	static ImVec4 sInfoButtonOnTint = ImVec4(0.0f, 0.431372549f, 0.05466f, 1.0f);
	static ImVec4 sWarningButtonOnTint = ImVec4(1.0f, 0.890196078f, 0.0588235294f, 1.0f);
	static ImVec4 sErrorButtonOnTint = ImVec4(1.0f, 0.309803922f, 0.309803922f, 1.0f);
	static ImVec4 sFatalButtonOnTint = ImVec4(1.0f, 0.309803922f, 0.309803922f, 1.0f);
	static ImVec4 sNoTint = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	static Ref<Texture2D> sTraceIcon;
	static Ref<Texture2D> sInfoIcon;
	static Ref<Texture2D> sWarnIcon;
	static Ref<Texture2D> sErrorIcon;
	static Ref<Texture2D> sFatalIcon;


	EditorConsolePanel::EditorConsolePanel()
	{
		RB_CORE_ASSERT(sInstance == nullptr);
		sInstance = this;
	}

	EditorConsolePanel::~EditorConsolePanel()
	{
		sInstance = nullptr;
	}

	void EditorConsolePanel::OnLoad()
	{
		// #TODO: Need a proper asset management system
		// Core shaders for example have to be used by all projects - Rebirth Reedit, Sandbox, games, etc.
		// Should have it so assets can be loaded from a working directory (the project) and the core engine directory (for things like shaders, missing texture image, etc)
		// Or perhaps best solution is to enforce a certain folder structure, since the plan is to have games be made via the editor anyway rather than a VS project
		// So could be:
		// - Engine installation folder
		// -- Rebirth-Reedit.exe
		// -- resources (or assets) // editor icons, missing texture, shaders, etc
		// - Game Project folder
		// -- game assets // game textures, scenes, audio files, etc
		sTraceIcon = Texture2D::Create("assets/icons/trace.png");
		sInfoIcon = Texture2D::Create("assets/icons/info.png");
		sWarnIcon = Texture2D::Create("assets/icons/warning.png");
		sErrorIcon = Texture2D::Create("assets/icons/error.png");
		sFatalIcon = Texture2D::Create("assets/icons/fatal.png");
	}

	void EditorConsolePanel::OnImguiRender()
	{
		//ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		static bool open = true;
		ImGui::Begin("Console", &open, ImGuiWindowFlags_NoScrollbar); // Console

		RenderMenu();
		ImGui::Separator();
		RenderConsole();

		ImGui::End(); // Console
		//ImGui::PopStyleColor();
	}

	void EditorConsolePanel::OnEvent(Event& e)
	{
		//RB_CORE_WARN("Event type: {}, data: {}", (int)e.GetType(), e.ToString());
		if (e.GetType() == EventType::SCENE_PRE_START)
		{
			if (mShouldClearOnPlay)
				mBufferBegin = 0;
		}
	}

	void EditorConsolePanel::RenderMenu()
	{
		ImVec4 traceButtonTint = (mMessageFilters & LogLevel_Trace) ? sTraceButtonOnTint : sNoTint;
		ImVec4 infoButtonTint = (mMessageFilters & LogLevel_Info) ? sInfoButtonOnTint : sNoTint;
		ImVec4 warnButtonTint = (mMessageFilters & LogLevel_Warning) ? sWarningButtonOnTint : sNoTint;
		ImVec4 errorButtonTint = (mMessageFilters & LogLevel_Error) ? sErrorButtonOnTint : sNoTint;
		ImVec4 fatalButtonTint = (mMessageFilters & LogLevel_Fatal) ? sFatalButtonOnTint : sNoTint;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 5));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(5, 3));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		ImGui::BeginChild("ConsoleToolbar", { 0, 40 }, false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding);

		if (ImGui::Button("Clear"))
		{
			mBufferBegin = 0;
		}

		// #TODO: Imgui helper methods for making label go on the left of the item
		ImGui::SameLine(0.0f, 5.0f);
		ImGui::TextUnformatted("Clear On Play:");
		ImGui::SameLine();
		ImGui::Checkbox("##ClearOnPlay", &mShouldClearOnPlay);

		//ImGui::SameLine(0.0f, 5.0f);
		//ImGui::TextUnformatted("Collapse:");
		//ImGui::SameLine();
		//ImGui::Checkbox("##CollapseMessages", &mCollapseMessages);

		constexpr float buttonOffset = 39;
		constexpr float rightSideOffset = 15;
#pragma warning(disable: 4312)
		ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 5) - rightSideOffset);
		if (ImGui::ImageButton((ImTextureID)sTraceIcon->GetId(), { 32, 32 }, { 0, 1 }, { 1, 0 }, -1, { 0, 0, 0, 0 }, traceButtonTint))
		{
			mMessageFilters ^= LogLevel_Trace;
		}

		ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 4) - rightSideOffset);
		if (ImGui::ImageButton((ImTextureID)sInfoIcon->GetId(), { 32, 32 }, { 0, 1 }, { 1, 0 }, -1, { 0, 0, 0, 0 }, infoButtonTint))
		{
			mMessageFilters ^= LogLevel_Info;
		}

		ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 3) - rightSideOffset);
		if (ImGui::ImageButton((ImTextureID)sWarnIcon->GetId(), { 32, 32 }, { 0, 1 }, { 1, 0 }, -1, { 0, 0, 0, 0 }, warnButtonTint))
		{
			mMessageFilters ^= LogLevel_Warning;
		}

		ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 2) - rightSideOffset);
		if (ImGui::ImageButton((ImTextureID)sErrorIcon->GetId(), { 32, 32 }, { 0, 1 }, { 1, 0 }, -1, { 0, 0, 0, 0 }, errorButtonTint))
		{
			mMessageFilters ^= LogLevel_Error;
		}

		ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 1) - rightSideOffset);
		if (ImGui::ImageButton((ImTextureID)sFatalIcon->GetId(), { 32, 32 }, { 0, 1 }, { 1, 0 }, -1, { 0, 0, 0, 0 }, fatalButtonTint))
		{
			mMessageFilters ^= LogLevel_Fatal;
		}
#pragma warning(default: 4312)

		ImGui::EndChild(); //ConsoleToolbar

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar(2);

	}

	void EditorConsolePanel::RenderConsole()
	{
		ImGui::PushStyleColor(ImGuiCol_TableRowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, ImVec4(0.11f, 0.11f, 0.11f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
		int colorsToPop = 3;
		ImGui::BeginChild("ConsoleChild");
		if(ImGui::BeginTable("LogMessages", 1, ImGuiTableFlags_RowBg))
		{
			colorsToPop = 2;
			if (!mBufferBegin || (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !mIsMessageInspectorHovered))
			{
				mDisplayMessageInspector = false;
				mSelectedMessage = nullptr;
			}

			for (uint32 i = 0; i < mBufferBegin; i++)
			{
				const auto& msg = mMessageBuffer[i];

				if (mMessageFilters & msg.GetLogLevel())
				{

					ImGui::TableNextColumn();

					std::string text = msg.GetMessage();

					ImGui::PushID(fmt::format("Copy_{}", i).c_str());
					if (ImGui::Button("Copy"))
					{
						ImGui::SetClipboardText(text.c_str());
					}
					ImGui::PopID();
					ImGui::SameLine();

					ImGui::PushID(fmt::format("Inspect_{}", i).c_str());
					if (ImGui::Button("Inspect")) // #TODO: Make magnifying glass image icon or something
					{
						mSelectedMessage = &mMessageBuffer[i];
						mDisplayMessageInspector = true;
					}
					ImGui::PopID();
					ImGui::SameLine();

#pragma warning(disable: 4312)
					if (msg.GetLogLevel() == LogLevel_Trace)
						ImGui::Image((ImTextureID)sTraceIcon->GetId(), { 24, 24 }, { 0, 1 }, { 1, 0 }, sTraceButtonOnTint);
					else if (msg.GetLogLevel() == LogLevel_Info)
						ImGui::Image((ImTextureID)sInfoIcon->GetId(), { 24, 24 }, { 0, 1 }, { 1, 0 }, sInfoButtonOnTint);
					else if (msg.GetLogLevel() == LogLevel_Warning)
						ImGui::Image((ImTextureID)sWarnIcon->GetId(), { 24, 24 }, { 0, 1 }, { 1, 0 }, sWarningButtonOnTint);
					else if (msg.GetLogLevel() == LogLevel_Error)
						ImGui::Image((ImTextureID)sErrorIcon->GetId(), { 24, 24 }, { 0, 1 }, { 1, 0 }, sErrorButtonOnTint);
					else if (msg.GetLogLevel() == LogLevel_Fatal)
						ImGui::Image((ImTextureID)sFatalIcon->GetId(), { 24, 24 }, { 0, 1 }, { 1, 0 }, sFatalButtonOnTint);
#pragma warning(default: 4312)
					ImGui::SameLine();



					size_t newLinePos = text.find_first_of('\n');
					size_t newLinePos2 = text.find_last_of('\n');
					if (newLinePos != std::string::npos && newLinePos != newLinePos2)
					{
						text.replace(newLinePos, text.length() - 1, " ...");
					}

					if (text.length() > 200)
					{
						size_t spacePos = text.find_first_of(' ', 200);
						if (spacePos != std::string::npos)
							text.replace(spacePos, text.length() - 1, " ...");
					}


					ImGui::TextWrapped(text.c_str());

					//if (mCollapseMessages && msg.GetCount() > 1)
					//{
					//	ImGui::SameLine(ImGui::GetWindowWidth() - 30);
					//	ImGui::Text("%d", msg.GetCount());
					//}

				}
			}

			if (mNewMessageAdded)
			{
				ImGui::SetScrollHereY(1.0f);
				mNewMessageAdded = false;
			}

			if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() && !mDisplayMessageInspector)
				ImGui::SetScrollHereY(1.0f);

			ImGui::PopStyleColor();
			if (mDisplayMessageInspector && mSelectedMessage != nullptr)
			{
				ImGui::Begin("Message Inspector");

				mIsMessageInspectorHovered = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();

				ImGui::PushTextWrapPos();
				const auto& msg = mSelectedMessage->GetMessage();
				ImGui::TextUnformatted(msg.c_str());
				ImGui::PopTextWrapPos();

				ImGui::End();
			}
			else
			{
				mIsMessageInspectorHovered = false;
			}

			ImGui::EndTable(); // LogMessages
		}

		ImGui::EndChild(); // ConsoleChild
		ImGui::PopStyleColor(colorsToPop);

	}

	void EditorConsolePanel::PushMessage(const ConsoleMessage& msg)
	{
		if (sInstance == nullptr)
			return;

		if (msg.GetLogLevel() == LogLevel_None)
			return;

		if (sInstance->mCollapseMessages)
		{
			for (auto& other : sInstance->mMessageBuffer)
			{
				if (msg.GetId() == other.GetId())
				{
					other.mCount++;
					return;
				}
			}
		}

		sInstance->mMessageBuffer[sInstance->mBufferBegin++] = msg;
		sInstance->mNewMessageAdded = true;

		if (sInstance->mBufferBegin == sMessageBufferCapacity)
			sInstance->mBufferBegin = 0;

	}

}

