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
// File Name: ImguiUtils.h
// Date File Created: 7/21/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

//#include <imgui.h>

namespace rebirth
{
	enum TextAlign
	{
		TextAlign_LEFT,
		TextAlign_RIGHT
	};

	class Texture2D;
}
namespace rebirth::UI
{
	// ImGui::Checkbox wrapper that makes it so the label appears on the left side of the checkbox
	bool Checkbox(const char* label, bool* value);

	// ImGui::SliderFloat3 but with unreal engine style colored X, Y, Z labels
	void DrawFloat3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f);

	// Draws a (?) and displays the given desc when that (?) is hovered
	void DrawTooltip(const char* desc);

	bool DrawFloatControl(const std::string& label, float* value, float step = 0.01f, float minValue = -0.0f, float maxValue = 0.0f);

	void Image(const Ref<Texture2D>& texture, const glm::vec2& size, glm::vec4 tintColor = { 1, 1, 1, 1 });
	bool ImageButton(const Ref<Texture2D>& texture, const glm::vec2& size, glm::vec4 tintColor = { 1, 1, 1, 1 });

	bool ColorEdit(const char* label, glm::vec4& color);

	bool Combobox(const char* label, const char* previewValue);

	void PushColumnWidth(float columnWidth);
	void PushTextAlign(TextAlign alignment);

	void PopColumnWidth();
	void PopTextAlign();
}

