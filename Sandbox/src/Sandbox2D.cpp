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
// File Name: Sandbox2D.cpp
// Date File Created: 6/24/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "Sandbox2D.h"

#include <imgui/imgui.h>


void Sandbox2D::OnAttach()
{
	RB_PROFILE_FUNC();
	mTexture = rebirth::Texture2D::Create("assets/textures/default.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(rebirth::Timestep ts)
{
	RB_PROFILE_FUNC();
	mCameraController.OnUpdate(ts);


	rebirth::RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1.0f });
	rebirth::RenderCommand::Clear();
	rebirth::Renderer2D::BeginScene(mCameraController.GetCamera());

	rebirth::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.6f, 0.3f, 0.15f, 1.0f }, { 0.8f, 0.8f });
	rebirth::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, mTexture, { 10.0f, 10.0f }, 5.0f, {1.0f, 0.5f, 0.5f, 1.0f});
	rebirth::Renderer2D::DrawRotatedQuad(mSquarePos, mSquareColor, glm::radians(mSquareAngle), mSquareSize);

	rebirth::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(rebirth::Event& e)
{
	mCameraController.OnEvent(e);
}

void Sandbox2D::OnImguiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorPicker4("Square Color", glm::value_ptr(mSquareColor));
	ImGui::DragFloat2("Square Position", glm::value_ptr(mSquarePos), 0.2f, -1.0f, 1.0f);
	ImGui::DragFloat2("Square Size", glm::value_ptr(mSquareSize), 0.2f, 0.05f, 10.0f);
	ImGui::DragFloat("Square Angle", &mSquareAngle, 0.2f, 0.0f, 360.0f);


	for (auto& res : mProfileResults)
	{
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, res.name);
		ImGui::Text(label, res.time);
	}

	mProfileResults.clear();

	ImGui::End();
}
