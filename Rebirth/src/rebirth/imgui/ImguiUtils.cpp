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

namespace rebirth::UI
{

	bool Checkbox(const char* label, bool* value)
	{
		ImGui::TextUnformatted(label);
		ImGui::SameLine();

		std::string temp = ReplaceAll(label, " ", "");
		return ImGui::Checkbox(fmt::format("##{}", temp).c_str(), value);
	}

}

