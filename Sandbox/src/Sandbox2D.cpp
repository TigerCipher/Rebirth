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
	float sq_verts[5 * 4] =
	{
		-0.5f, -0.5f, 0.0f,	
		0.5f, -0.5f, 0.0f,	
		0.5f, 0.5f, 0.0f,	
		-0.5f, 0.5f, 0.0f
	};

	mSquareVtxArray = rebirth::VertexArray::Create();

	Ref<rebirth::VertexBuffer> svb = rebirth::VertexBuffer::Create(sizeof(sq_verts), sq_verts);
	svb->SetLayout({
			{ rebirth::ShaderDataType::FLOAT3, "aPos" }
		});
	mSquareVtxArray->AddVertexBuffer(svb);

	uint32_t sq_indices[6] = { 0, 1, 2, 2, 3, 0 };
	Ref<rebirth::IndexBuffer> sib = rebirth::IndexBuffer::Create(sizeof(sq_indices) / sizeof(uint32_t), sq_indices);

	mSquareVtxArray->SetIndexBuffer(sib);


	mShader = rebirth::Shader::Create("assets/shaders/FlatColor.glsl");

}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(rebirth::Timestep ts)
{
	mCameraController.OnUpdate(ts);

	rebirth::RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1.0f });
	rebirth::RenderCommand::Clear();

	rebirth::Renderer::BeginScene(mCameraController.GetCamera());

	mShader->Bind();
	std::dynamic_pointer_cast<rebirth::OpenGLShader>(mShader)->SetUniformVec4("uColor", mSquareColor);
	rebirth::Renderer::Submit(mShader, mSquareVtxArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


	rebirth::Renderer::EndScene();
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
