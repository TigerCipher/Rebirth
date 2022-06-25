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
#include "RenderCommand.h"

namespace rebirth
{

	struct Storage
	{
		Ref<VertexArray> vertexArray;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;
	};

	static Storage* sStorage;

	void Renderer2D::Init()
	{
		RB_PROFILE_FUNC();
		sStorage = new Storage();
		float sq_verts[5 * 4] =
		{
			/* Positions              Tex coords*/
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f
		};

		sStorage->vertexArray = VertexArray::Create();

		Ref<VertexBuffer> svb = VertexBuffer::Create(sizeof(sq_verts), sq_verts);
		svb->SetLayout({
				{ ShaderDataType::FLOAT3, "aPos" },
				{ ShaderDataType::FLOAT2, "aTexCoord" },
			});
		sStorage->vertexArray->AddVertexBuffer(svb);

		uint32_t sq_indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> sib = IndexBuffer::Create(sizeof(sq_indices) / sizeof(uint32_t), sq_indices);

		sStorage->vertexArray->SetIndexBuffer(sib);

		sStorage->whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteData = 0xffffffff;
		sStorage->whiteTexture->SetData(&whiteData, sizeof(uint32_t));

		sStorage->textureShader = Shader::Create("assets/shaders/Texture.vert", "assets/shaders/Texture.frag");
		sStorage->textureShader->Bind();
		sStorage->textureShader->SetInt("uTexture", 0);
	}

	void Renderer2D::Shutdown()
	{
		RB_PROFILE_FUNC();
		if (sStorage)
		{
			delete sStorage;
			sStorage = nullptr;
		}
	}

	void Renderer2D::BeginScene(const OrthoCamera& camera)
	{
		RB_PROFILE_FUNC();
		sStorage->textureShader->Bind();
		sStorage->textureShader->SetMat4("uViewProj", camera.ViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		RB_PROFILE_FUNC();
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec4& color, const glm::vec2& size)
	{
		DrawQuad({ pos.x, pos.y, 0.0f }, color, size);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec4& color, const glm::vec2& size)
	{
		RB_PROFILE_FUNC();
		sStorage->textureShader->SetFloat4("uColor", color);
		sStorage->textureShader->SetFloat("uTilingFactor", 1.0f);
		sStorage->whiteTexture->Bind();

		// translation * rotation * scale
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

		sStorage->textureShader->SetMat4("uModelTransform", transform);

		sStorage->vertexArray->Bind();
		RenderCommand::DrawIndexed(sStorage->vertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const Ref<Texture2D>& texture, const glm::vec2& size, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ pos.x, pos.y, 0.0f }, texture, size, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const Ref<Texture2D>& texture, const glm::vec2& size, float tilingFactor, const glm::vec4& tintColor)
	{
		RB_PROFILE_FUNC();
		sStorage->textureShader->SetFloat4("uColor", tintColor);
		sStorage->textureShader->SetFloat("uTilingFactor", tilingFactor);
		texture->Bind();

		// translation * rotation * scale
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		sStorage->textureShader->SetMat4("uModelTransform", transform);


		sStorage->vertexArray->Bind();
		RenderCommand::DrawIndexed(sStorage->vertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec4& color, float angle, const glm::vec2& size /*= {1.0f, 1.0f}*/)
	{
		DrawRotatedQuad({ pos.x, pos.y, 0.0f }, color, angle, size);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec4& color, float angle, const glm::vec2& size /*= {1.0f, 1.0f}*/)
	{
		RB_PROFILE_FUNC();
		sStorage->textureShader->SetFloat4("uColor", color);
		sStorage->textureShader->SetFloat("uTilingFactor", 1.0f);
		sStorage->whiteTexture->Bind();

		// translation * rotation * scale
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), angle, { 0, 0, 1 })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		sStorage->textureShader->SetMat4("uModelTransform", transform);

		sStorage->vertexArray->Bind();
		RenderCommand::DrawIndexed(sStorage->vertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const Ref<Texture2D>& texture, float angle, const glm::vec2& size, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ pos.x, pos.y, 0.0f }, texture, angle, size, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const Ref<Texture2D>& texture, float angle, const glm::vec2& size, float tilingFactor, const glm::vec4& tintColor)
	{
		RB_PROFILE_FUNC();
		// #TODO: Create overloaded method for the color tint
		sStorage->textureShader->SetFloat4("uColor", tintColor);
		sStorage->textureShader->SetFloat("uTilingFactor", tilingFactor);
		texture->Bind();

		// translation * rotation * scale
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), angle, {0, 0, 1})
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		sStorage->textureShader->SetMat4("uModelTransform", transform);


		sStorage->vertexArray->Bind();
		RenderCommand::DrawIndexed(sStorage->vertexArray);
	}

}

