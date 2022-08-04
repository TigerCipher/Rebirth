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
// File Name: AssetLayer.h
// Date File Created: 7/29/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include <Rebirth.h>

#include <imgui/imgui.h>

namespace rebirth
{
	class TextureLoader : public AssetLoader
	{
	public:
		bool Load(AssetMetadata& meta, Reference<Asset>& asset) override
		{
			asset = Texture2D::Create(meta.path.string(), 0);
			bool res = asset.As<Texture2D>()->IsLoaded();
			if(!res)
				asset->SetFlags(AssetFlag_Invalid);
			return res;
		}
		bool Save(Reference<Asset>& asset) override
		{
			return false;
		}
	};

	class AssetLayer : public Layer
	{
	public:
		virtual ~AssetLayer() = default;
		void OnAttach() override
		{
			AssetManager::Init();
			AssetManager::RegisterLoader<TextureLoader>(AssetType_Texture);

			const UUID testTexture = AssetManager::ImportAsset("assets/textures/default.png");
			mTexture = AssetManager::GetAsset<Texture2D>(testTexture);

			std::string virtualDir = "assets/";
			RbaArchive rbaFile("test.rba");
			rbaFile.CreateRbaFile("assets", virtualDir, 1, true, 0);
			//rbaFile.ExtractRbaFile("exported_assets/");

			PhysicalFile testFile("test/dir/just_a_test.txt", FileMode_Append | FileMode_Read);
			if(testFile.Open())
			{
				testFile.WriteLine("Testing out the writing line method");
				testFile.WriteFloat(3.2343545f);

				std::vector<byte> testVec(26);
				char a = 'a';
				for(auto& t : testVec)
				{
					t = a++;
				}

				testFile.Write(testVec.data(), testVec.size());

				std::vector<byte> readVec;
				testFile.ReadAllBytes(readVec);

				for (const auto& b : readVec)
				{
					fmt::print("{}", (char)b);
				}

			}
			else
			{
				RB_CORE_ERROR("Failed to open file");
			}

		}
		void OnDetach() override{}
		void OnUpdate(Timestep timestep) override
		{
			RenderCommand::SetClearColor({ 0.2f, 0.0f, 0.0f, 1.0f });
			RenderCommand::Clear();
		}
		void OnImguiRender() override
		{
			ImGui::Begin("Test");

			//UI::Image(mTexture, { 512, 512 });
			ImGui::Image(reinterpret_cast<ImTextureID>(mTexture->GetId()), { 512, 512 }, { 0, 1 }, { 1, 0 });

			ImGui::End();
		}
		void OnEvent(Event& e) override{}

	private:
		Reference<Texture2D> mTexture;
	};
}