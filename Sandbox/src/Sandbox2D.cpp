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

	rebirth::FramebufferSpecification spec;
	spec.width = 1920;
	spec.height = 1080;
	//mFramebuffer = rebirth::Framebuffer::Create(spec);
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(rebirth::Timestep ts)
{
	RB_PROFILE_FUNC();
	mCameraController.OnUpdate(ts);

	rebirth::Renderer2D::ResetStats();

	//mFramebuffer->Bind();

	rebirth::RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1.0f });
	rebirth::RenderCommand::Clear();


	static float rot = 0.0f;
	rot += ts * 40.0f;

	rebirth::Renderer2D::BeginScene(mCameraController.GetCamera());

	rebirth::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.6f, 0.3f, 0.15f, 1.0f }, { 0.8f, 0.8f });
	rebirth::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.2f, 0.5f, 0.7f, 1.0f }, { 0.8f, 0.8f });
	rebirth::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, mTexture, { 20.0f, 20.0f }, 5.0f);
	rebirth::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.0f }, mTexture, glm::radians(rot), { 1.0f, 1.0f }, 25.0f);
	rebirth::Renderer2D::DrawRotatedQuad({-1.5f, 0.75f}, mSquareColor, glm::radians(mSquareAngle), mSquareSize);

	for (float y = -5.0f; y < 5.0f; y += 0.5f)
	{
		for (float x = -5.0f; x < 5.0f; x += 0.5f)
		{
			glm::vec4 rgb = { (x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 0.2f, 0.75f };
			rebirth::Renderer2D::DrawQuad({ x, y }, rgb, { 0.45f, 0.45f });
		}
	}

	rebirth::Renderer2D::EndScene();

	//mFramebuffer->Unbind();

}

void Sandbox2D::OnEvent(rebirth::Event& e)
{
	mCameraController.OnEvent(e);
}

void Sandbox2D::OnImguiRender()
{
		ImGui::Begin("Settings");

		auto stats = rebirth::Renderer2D::GetStats();
		//ImGui::Begin("Statistics");
		ImGui::Text("Render Batch Stats:");
		ImGui::Text("Draw Calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quads);
		ImGui::Text("Vertex Count: %d", stats.GetVertCount());
		ImGui::Text("Index Count: %d", stats.GetIndicesCount());

		ImGui::ColorPicker4("Square Color", glm::value_ptr(mSquareColor));

		uint32_t textureID = mTexture->GetId();
		ImGui::Image((void*)textureID, ImVec2{ 256.0f, 256.0f });
		ImGui::End();

}
