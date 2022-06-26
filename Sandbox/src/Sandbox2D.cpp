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
	mSpritesheet = rebirth::Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");
	mStairsSprite = rebirth::SubTexture2D::CreateFromCoords(mSpritesheet, { 7, 6 }, { 128, 128 });
	mBarrelSprite = rebirth::SubTexture2D::CreateFromCoords(mSpritesheet, { 8, 2 }, { 128, 128 });
	mTreeSprite = rebirth::SubTexture2D::CreateFromCoords(mSpritesheet, { 2, 1 }, { 128, 128 }, {1, 2});

	mParticle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	mParticle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	mParticle.SizeBegin = 0.5f, mParticle.SizeVariation = 0.3f, mParticle.SizeEnd = 0.0f;
	mParticle.LifeTime = 5.0f;
	mParticle.Velocity = { 0.0f, 0.0f };
	mParticle.VelocityVariation = { 3.0f, 1.0f };
	mParticle.Position = { 0.0f, 0.0f };
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

#if 0
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
#endif

	if (rebirth::Input::IsMouseButtonPressed(RB_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = rebirth::Input::GetMousePos();
		auto width = rebirth::Application::Instance().GetWindow().GetWidth();
		auto height = rebirth::Application::Instance().GetWindow().GetHeight();

		auto bounds = mCameraController.GetBounds();
		auto pos = mCameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		mParticle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 10; i++)
			mParticleSystem.Emit(mParticle);
	}

	mParticleSystem.OnUpdate(ts);
	mParticleSystem.OnRender(mCameraController.GetCamera());

	rebirth::Renderer2D::BeginScene(mCameraController.GetCamera());
	rebirth::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.5f }, mStairsSprite);
	rebirth::Renderer2D::DrawQuad({ 1.0f, 0.0f, 0.5f }, mBarrelSprite);
	rebirth::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.5f }, mTreeSprite, {1.0f, 2.0f});
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
	ImGui::DragFloat("Square Angle", &mSquareAngle, 0.2f, -360.0f, 360.0f);
	ImGui::End();

	auto stats = rebirth::Renderer2D::GetStats();
	ImGui::Begin("Statistics");
	ImGui::Text("Render Batch Stats:");
	ImGui::Text("Draw Calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quads);
	ImGui::Text("Vertex Count: %d", stats.GetVertCount());
	ImGui::Text("Index Count: %d", stats.GetIndicesCount());
	ImGui::End();
}
