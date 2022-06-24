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

#include <platform/opengl/OpenGLShader.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Sandbox2D::OnAttach()
{


}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(rebirth::Timestep ts)
{
	mCameraController.OnUpdate(ts);

	rebirth::RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1.0f });
	rebirth::RenderCommand::Clear();

	rebirth::Renderer2D::BeginScene(mCameraController.GetCamera());

	rebirth::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, mSquareColor);
	//mShader->Bind();
	//std::dynamic_pointer_cast<rebirth::OpenGLShader>(mShader)->SetUniformVec4("uColor", mSquareColor);

	rebirth::Renderer2D::EndScene();
}

void Sandbox2D::OnEvent(rebirth::Event& e)
{
	mCameraController.OnEvent(e);
}

void Sandbox2D::OnImguiRender()
{
	ImGui::Begin("Color Changer");
	ImGui::ColorPicker4("Square Color", glm::value_ptr(mSquareColor));
	ImGui::End();
}
