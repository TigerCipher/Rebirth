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
// File Name: ImguiUtils.cpp
// Date File Created: 7/21/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "ImguiUtils.h"
#include "rebirth/util/StringUtil.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace rebirth::UI
{

	static float gColumnWidth = 120.0f;
	static TextAlign gAlignment = TextAlign_LEFT;

	bool Checkbox(const char* label, bool* value)
	{
		ImGui::TextUnformatted(label);
		ImGui::SameLine();

		std::string temp = ReplaceAll(label, " ", "");
		return ImGui::Checkbox(fmt::format("##{}", temp).c_str(), value);
	}

	void DrawFloat3Control(const std::string& label, glm::vec3& values, float resetValue /*= 0.0f*/)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		if (ImGui::BeginTable(label.c_str(), 2))
		{
			//ImGui::Columns(2);
			//ImGui::SetColumnWidth(0, columnWidth);
			ImGui::TableSetupColumn("col1", ImGuiTableColumnFlags_WidthFixed, gColumnWidth);
			ImGui::TableNextColumn();

			if (gAlignment == TextAlign_RIGHT)
			{
				auto posX = (ImGui::GetCursorPosX() + gColumnWidth - ImGui::CalcTextSize(label.c_str()).x
					- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
				ImGui::SetCursorPosX(posX);
			}
			ImGui::Text(label.c_str());
			ImGui::TableNextColumn();

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.7f, 0.1f, 0.1f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.5f, 0.1f, 0.1f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", buttonSize))
				values.x = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.3f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.65f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.75f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.45f, 0.2f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", buttonSize))
				values.y = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.3f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.1f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.6f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Z", buttonSize))
				values.z = resetValue;
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.3f");
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();

			//ImGui::Columns(1);

			ImGui::EndTable();
		}
	}

	void DrawTooltip(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	// #TODO Make a column aligned version
	bool DrawFloatControl(const std::string& label, float* value, float step /*= 0.01f*/, float minValue /*= 0.0f*/, float maxValue /*= 0.0f*/)
	{
		bool ret = false;
		if (ImGui::BeginTable(label.c_str(), 2))
		{
			//ImGui::SetColumnWidth(0, 120.0f);
			ImGui::TableSetupColumn("col1", ImGuiTableColumnFlags_WidthFixed, gColumnWidth);
			ImGui::TableNextColumn();
			if (gAlignment == TextAlign_RIGHT)
			{
				auto posX = (ImGui::GetCursorPosX() + gColumnWidth - ImGui::CalcTextSize(label.c_str()).x
					- ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
				//if (posX > ImGui::GetCursorPosX())
				ImGui::SetCursorPosX(posX);
			}
			ImGui::TextUnformatted(label.c_str());
			ImGui::TableNextColumn();
			//ImGui::SameLine();
			std::string temp = ReplaceAll(label, " ", "");
			ret = ImGui::DragFloat(fmt::format("##{}", temp).c_str(), value, step, minValue, maxValue, "%.3f");

			ImGui::EndTable();
		}
		return ret;
	}

	void PushColumnWidth(float columnWidth)
	{
		gColumnWidth = columnWidth;
	}

	void PushTextAlign(TextAlign alignment)
	{
		gAlignment = alignment;
	}

	void PopColumnWidth()
	{
		gColumnWidth = 120.0f;
	}

	void PopTextAlign()
	{
		gAlignment = TextAlign_LEFT;
	}

}

