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
		mDirectoryIcon = Texture2D::Create("assets/icons/directory.png");
		mFileIcon = Texture2D::Create("assets/icons/file.png");
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

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int colCount = (int)(panelWidth / cellSize);
		if (colCount < 1)
			colCount = 1;

		ImGui::Columns(colCount, 0, false);

		for (auto& dirEntry : std::filesystem::directory_iterator(mCurrentDir))
		{
			const auto& path = dirEntry.path();
			auto relPath = std::filesystem::relative(path, sAssetsPath);
			std::string filenameStr = relPath.filename().string();

			Ref<Texture2D> icon = dirEntry.is_directory() ? mDirectoryIcon : mFileIcon;
			if (path.parent_path().string().find("textures") != std::string::npos)
			{
				icon = Texture2D::Create(path.string());
			//RB_CORE_TRACE(path.extension());
			}
			ImGui::ImageButton((ImTextureID)icon->GetId(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (dirEntry.is_directory())
					mCurrentDir /= path.filename();
			}
			ImGui::TextWrapped(filenameStr.c_str());
			ImGui::NextColumn();
		}

		ImGui::Columns(1);
		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End(); // Content Browser
	}

}

