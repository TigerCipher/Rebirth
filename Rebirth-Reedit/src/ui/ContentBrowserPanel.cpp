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
// File Name: ContentBrowserPanel.cpp
// Date File Created: 7/13/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

namespace rebirth
{

	// TODO #FUTURE: Asset directory should be in relation to 'projects'
	static const std::filesystem::path sAssetsPath = "assets";


	ContentBrowserPanel::ContentBrowserPanel() :
		mCurrentDir(sAssetsPath)
	{

	}

	void ContentBrowserPanel::OnImguiRender()
	{
		ImGui::Begin("Content Browser");

		if (mCurrentDir != sAssetsPath)
		{
			if (ImGui::Button("<-"))
			{
				mCurrentDir = mCurrentDir.parent_path();
			}
		}

		for (auto& dirEntry : std::filesystem::directory_iterator(mCurrentDir))
		{
			const auto& path = dirEntry.path();
			auto relPath = std::filesystem::relative(path, sAssetsPath);
			std::string filenameStr = relPath.filename().string();

			if (dirEntry.is_directory())
			{
				if (ImGui::Button(filenameStr.c_str()))
				{
					mCurrentDir /= path.filename();
				}
			}
			else
			{
				if (ImGui::Button(filenameStr.c_str()))
				{ }
			}
		}

		ImGui::End(); // Content Browser
	}

}

