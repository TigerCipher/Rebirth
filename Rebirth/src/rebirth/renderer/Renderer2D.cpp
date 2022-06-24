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
// File Name: Renderer2D.cpp
// Date File Created: 6/24/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "platform/opengl/OpenGLShader.h"
#include "RenderCommand.h"

namespace rebirth
{

	struct Storage
	{
		Ref<VertexArray> vertexArray;
		Ref<Shader> shader;
	};

	static Storage* sStorage;

	void Renderer2D::Init()
	{
		sStorage = new Storage();
		float sq_verts[3 * 4] =
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};

		sStorage->vertexArray = VertexArray::Create();

		Ref<VertexBuffer> svb = VertexBuffer::Create(sizeof(sq_verts), sq_verts);
		svb->SetLayout({
				{ ShaderDataType::FLOAT3, "aPos" }
			});
		sStorage->vertexArray->AddVertexBuffer(svb);

		uint32_t sq_indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> sib = IndexBuffer::Create(sizeof(sq_indices) / sizeof(uint32_t), sq_indices);

		sStorage->vertexArray->SetIndexBuffer(sib);


		sStorage->shader = Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		if (sStorage)
		{
			delete sStorage;
			sStorage = nullptr;
		}
	}

	void Renderer2D::BeginScene(const OrthoCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(sStorage->shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(sStorage->shader)->SetUniformMat4("uViewProj", camera.ViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(sStorage->shader)->SetUniformMat4("uModelTransform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ pos.x, pos.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color)
	{
		sStorage->shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(sStorage->shader)->SetUniformVec4("uColor", color);

		sStorage->vertexArray->Bind();
		RenderCommand::DrawIndexed(sStorage->vertexArray);
	}

}

