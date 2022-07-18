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

#include <imgui.h>
#include <imgui_internal.h>

#include <bitset>

namespace rebirth
{
	static EditorConsolePanel* sInstance = nullptr;

	static ImVec4 sTraceButtonOnTint = ImVec4(0.431372549f, 0.431372549f, 0.431372549f, 1.0f);
	static ImVec4 sInfoButtonOnTint = ImVec4(0.0f, 0.431372549f, 0.05466f, 1.0f);
	static ImVec4 sWarningButtonOnTint = ImVec4(1.0f, 0.890196078f, 0.0588235294f, 1.0f);
	static ImVec4 sErrorButtonOnTint = ImVec4(1.0f, 0.309803922f, 0.309803922f, 1.0f);
	static ImVec4 sFatalButtonOnTint = ImVec4(1.0f, 0.309803922f, 0.309803922f, 1.0f);
	static ImVec4 sNoTint = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	static Ref<Texture2D> sTraceIcon = nullptr;
	static Ref<Texture2D> sInfoIcon = nullptr;
	static Ref<Texture2D> sWarnIcon = nullptr;
	static Ref<Texture2D> sErrorIcon = nullptr;
	static Ref<Texture2D> sFatalIcon = nullptr;


	EditorConsolePanel::EditorConsolePanel()
	{
		RB_CORE_ASSERT(sInstance == nullptr);

		sTraceIcon = Texture2D::Create("assets/icons/trace.png");
		sInfoIcon = Texture2D::Create("assets/icons/info.png");
		sWarnIcon = Texture2D::Create("assets/icons/warning.png");
		sErrorIcon = Texture2D::Create("assets/icons/error.png");
		sFatalIcon = Texture2D::Create("assets/icons/fatal.png");

		sInstance = this;
	}

	EditorConsolePanel::~EditorConsolePanel()
	{
		sInstance = nullptr;
	}

	void EditorConsolePanel::OnImguiRender()
	{
		ImGui::Begin("Console"); // Console

		RenderMenu();
		ImGui::Separator();
		RenderConsole();

		ImGui::End(); // Console
	}

	void EditorConsolePanel::OnEvent(Event& e)
	{

	}

	void EditorConsolePanel::RenderMenu()
	{
		ImVec4 traceButtonTint = (mMessageFilters & (int16)ConsoleMessage::Category::Category_Trace) ? sTraceButtonOnTint : sNoTint;
		ImVec4 infoButtonTint = (mMessageFilters & (int16)ConsoleMessage::Category::Category_Info) ? sInfoButtonOnTint : sNoTint;
		ImVec4 warnButtonTint = (mMessageFilters & (int16)ConsoleMessage::Category::Category_Warning) ? sWarningButtonOnTint : sNoTint;
		ImVec4 errorButtonTint = (mMessageFilters & (int16)ConsoleMessage::Category::Category_Error) ? sErrorButtonOnTint : sNoTint;
		ImVec4 fatalButtonTint = (mMessageFilters & (int16)ConsoleMessage::Category::Category_Fatal) ? sFatalButtonOnTint : sNoTint;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 5));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(5, 3));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		if (ImGui::Button("Clear"))
		{
			//RB_CORE_ERROR("Message filters: {}", std::bitset<8>(mMessageFilters).to_string());
			mBufferBegin = 0;
		}

		// #TODO: Imgui helper methods for making label go on the left of the item
		ImGui::SameLine(0.0f, 5.0f);
		ImGui::TextUnformatted("Clear On Play:");
		ImGui::SameLine();
		ImGui::Checkbox("##ClearOnPlay", &mShouldClearOnPlay);

		ImGui::SameLine(0.0f, 5.0f);
		ImGui::TextUnformatted("Collapse:");
		ImGui::SameLine();
		ImGui::Checkbox("##CollapseMessages", &mCollapseMessages);

		constexpr float buttonOffset = 39;
		constexpr float rightSideOffset = 15;

		// #TODO: Fix texture coords

		ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 5) - rightSideOffset);
		if (ImGui::ImageButton((ImTextureID)sTraceIcon->GetId(), { 24, 24 }, { 0, 0 }, { 1, 1 }, -1, { 0, 0, 0, 0 }, traceButtonTint))
		{
			mMessageFilters ^= (int16)ConsoleMessage::Category::Category_Trace;
		}

		ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 4) - rightSideOffset);
		if (ImGui::ImageButton((ImTextureID)sInfoIcon->GetId(), { 24, 24 }, { 0, 0 }, { 1, 1 }, -1, { 0, 0, 0, 0 }, infoButtonTint))
		{
			mMessageFilters ^= (int16)ConsoleMessage::Category::Category_Info;
		}

		ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 3) - rightSideOffset);
		if (ImGui::ImageButton((ImTextureID)sWarnIcon->GetId(), { 24, 24 }, { 0, 0 }, { 1, 1 }, -1, { 0, 0, 0, 0 }, warnButtonTint))
		{
			mMessageFilters ^= (int16)ConsoleMessage::Category::Category_Warning;
		}

		ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 2) - rightSideOffset);
		if (ImGui::ImageButton((ImTextureID)sErrorIcon->GetId(), { 24, 24 }, { 0, 0 }, { 1, 1 }, -1, { 0, 0, 0, 0 }, errorButtonTint))
		{
			mMessageFilters ^= (int16)ConsoleMessage::Category::Category_Error;
		}

		ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 1) - rightSideOffset);
		if (ImGui::ImageButton((ImTextureID)sFatalIcon->GetId(), { 24, 24 }, { 0, 0 }, { 1, 1 }, -1, { 0, 0, 0, 0 }, fatalButtonTint))
		{
			mMessageFilters ^= (int16)ConsoleMessage::Category::Category_Fatal;
		}

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar(2);

	}

	void EditorConsolePanel::RenderConsole()
	{
		ImGui::BeginChild("LogMessages");

		if (!mBufferBegin || (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !mIsMessageInspectorHovered))
		{
			mDisplayMessageInspector = false;
			mSelectedMessage = nullptr;
		}

		for (uint32 i = 0; i < mBufferBegin; i++)
		{
			const auto& msg = mMessageBuffer[i];

			if (mMessageFilters & (int16)msg.GetCategory())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 5));

				if(i % 2 == 0)
					ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.20f, 0.20f, 0.20f, 1.0f));
				ImGui::BeginChild(i + 1, ImVec2(0, ImGui::GetFontSize() * 1.75F), false,
					ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding);

				if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				{
					mSelectedMessage = &mMessageBuffer[i];
					mDisplayMessageInspector = true;
				}

				std::string text = msg.GetMessage();

				if (ImGui::BeginPopupContextWindow())
				{
					if (ImGui::MenuItem("Copy"))
						ImGui::SetClipboardText(text.c_str());
					ImGui::EndPopup();
				}

				// #TODO: Fix texture coords
				if (msg.GetCategory() == ConsoleMessage::Category::Category_Trace)
					ImGui::Image((ImTextureID)sTraceIcon->GetId(), { 24, 24 }, { 0, 0 }, { 1, 1 }, sTraceButtonOnTint);
				else if (msg.GetCategory() == ConsoleMessage::Category::Category_Info)
					ImGui::Image((ImTextureID)sInfoIcon->GetId(), { 24, 24 }, { 0, 0 }, { 1, 1 }, sInfoButtonOnTint);
				else if (msg.GetCategory() == ConsoleMessage::Category::Category_Warning)
					ImGui::Image((ImTextureID)sWarnIcon->GetId(), { 24, 24 }, { 0, 0 }, { 1, 1 }, sWarningButtonOnTint);
				else if (msg.GetCategory() == ConsoleMessage::Category::Category_Error)
					ImGui::Image((ImTextureID)sErrorIcon->GetId(), { 24, 24 }, { 0, 0 }, { 1, 1 }, sErrorButtonOnTint);
				else if (msg.GetCategory() == ConsoleMessage::Category::Category_Fatal)
					ImGui::Image((ImTextureID)sFatalIcon->GetId(), { 24, 24 }, { 0, 0 }, { 1, 1 }, sFatalButtonOnTint);

				ImGui::SameLine();

				if (text.length() > 200)
				{
					size_t spacePos = text.find_first_of(' ', 200);
					if (spacePos != std::string::npos)
						text.replace(spacePos, text.length() - 1, "...");
				}

				ImGui::TextUnformatted(text.c_str());

				if (mCollapseMessages && msg.GetCount() > 1)
				{
					ImGui::SameLine(ImGui::GetWindowWidth() - 30);
					ImGui::Text("%d", msg.GetCount());
				}

				ImGui::EndChild(); // i + 1

				if (i % 2 == 0)
					ImGui::PopStyleColor();

				ImGui::PopStyleVar();

			}
		}

		if (mNewMessageAdded)
		{
			ImGui::SetScrollHereY(1.0f);
			mNewMessageAdded = false;
		}

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() && !mDisplayMessageInspector)
			ImGui::SetScrollHereY(1.0f);

		if (mDisplayMessageInspector && mSelectedMessage != nullptr)
		{
			ImGui::Begin("Message Inspector");

			mIsMessageInspectorHovered = ImGui::IsWindowHovered();

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

		ImGui::EndChild(); // LogMessages


	}

	void EditorConsolePanel::PushMessage(const ConsoleMessage& msg)
	{
		if (sInstance == nullptr)
			return;

		if (msg.GetCategory() == ConsoleMessage::Category::Category_None)
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

