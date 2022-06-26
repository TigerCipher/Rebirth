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

	rebirth::Renderer2D::ResetStats();

	rebirth::RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1.0f });
	rebirth::RenderCommand::Clear();


	static float rot = 0.0f;
	rot += ts * 40.0f;

	rebirth::Renderer2D::BeginScene(mCameraController.GetCamera());

	rebirth::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.6f, 0.3f, 0.15f, 1.0f }, { 0.8f, 0.8f });
	rebirth::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.2f, 0.5f, 0.7f, 1.0f }, { 0.8f, 0.8f });
	rebirth::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, mTexture, { 20.0f, 20.0f }, 5.0f);
	rebirth::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.0f }, mTexture, glm::radians(rot), { 1.0f, 1.0f }, 25.0f);
	rebirth::Renderer2D::DrawRotatedQuad(mSquarePos, mSquareColor, glm::radians(mSquareAngle), mSquareSize);

	for (float y = -5.0f; y < 5.0f; y += 0.5f)
	{
		for (float x = -5.0f; x < 5.0f; x += 0.5f)
		{
			glm::vec4 rgb = { (x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 0.2f, 0.75f };
			rebirth::Renderer2D::DrawQuad({ x, y }, rgb, { 0.45f, 0.45f });
		}
	}

	rebirth::Renderer2D::EndScene();

}

void Sandbox2D::OnEvent(rebirth::Event& e)
{
	mCameraController.OnEvent(e);
}

void Sandbox2D::OnImguiRender()
{
	// Note: Switch this to true to enable dockspace
	static bool dockingEnabled = true;
	if (dockingEnabled)
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) rebirth::Application::Instance().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

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

		ImGui::End();
	}
	else
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

		ImGui::End();
	}
}
