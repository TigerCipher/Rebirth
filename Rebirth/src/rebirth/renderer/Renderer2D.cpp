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

	struct QuadVertex
	{
		glm::vec3 pos;
		glm::vec4 color;
		glm::vec2 texCoord;
		float texIndex;
		float tilingFactor;
	};

	struct RenderData
	{
		const uint32_t MAX_QUADS = 10000;
		const uint32_t MAX_VERTS = MAX_QUADS * 4;
		const uint32_t MAX_INDICES = MAX_QUADS * 6;
		static const uint32_t MAX_TEXTURE_SLOTS = 32; // #TODO: Detect max texture slots from gpu?

		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;

		uint quadIndexCount = 0;

		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MAX_TEXTURE_SLOTS> textureSlots;
		uint32_t textureSlotIndex = 1; // we use 0 for our default white texture

		glm::vec4 quadVertexPos[4];
	};

	static RenderData sData;

	

	void Renderer2D::Init()
	{
		RB_PROFILE_FUNC();
		sData.vertexArray = VertexArray::Create();

		sData.vertexBuffer = VertexBuffer::Create(sData.MAX_VERTS * sizeof(QuadVertex));
		sData.vertexBuffer->SetLayout({
				{ ShaderDataType::FLOAT3, "aPos" },
				{ ShaderDataType::FLOAT4, "aColor" },
				{ ShaderDataType::FLOAT2, "aTexCoord" },
				{ ShaderDataType::FLOAT, "aTexIndex" },
				{ ShaderDataType::FLOAT, "aTilingFactor" },
			});
		sData.vertexArray->AddVertexBuffer(sData.vertexBuffer);

		sData.quadVertexBufferBase = new QuadVertex[sData.MAX_VERTS];

		uint32_t* quadIndices = new uint32_t[sData.MAX_INDICES];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < sData.MAX_INDICES; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> sib = IndexBuffer::Create(sData.MAX_INDICES, quadIndices);
		sData.vertexArray->SetIndexBuffer(sib);
		delete[] quadIndices;

		sData.whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteData = 0xffffffff;
		sData.whiteTexture->SetData(&whiteData, sizeof(uint32_t));

		int32_t samplers[sData.MAX_TEXTURE_SLOTS];
		for (uint i = 0; i < sData.MAX_TEXTURE_SLOTS; i++)
		{
			samplers[i] = i;
		}

		sData.textureShader = Shader::Create("assets/shaders/Texture.vert", "assets/shaders/Texture.frag");
		sData.textureShader->Bind();
		sData.textureShader->SetIntArray("uTextures", samplers, sData.MAX_TEXTURE_SLOTS);

		sData.textureSlots[0] = sData.whiteTexture;

		sData.quadVertexPos[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		sData.quadVertexPos[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		sData.quadVertexPos[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		sData.quadVertexPos[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		//RB_PROFILE_FUNC();
	}

	void Renderer2D::BeginScene(const OrthoCamera& camera)
	{
		RB_PROFILE_FUNC();
		sData.textureShader->Bind();
		sData.textureShader->SetMat4("uViewProj", camera.ViewProjectionMatrix());
		sData.quadIndexCount = 0;
		sData.quadVertexBufferPtr = sData.quadVertexBufferBase;
	}

	void Renderer2D::EndScene()
	{
		RB_PROFILE_FUNC();
		uint32_t size = (uint8_t*) sData.quadVertexBufferPtr - (uint8_t*)sData.quadVertexBufferBase;
		sData.vertexBuffer->SetData(sData.quadVertexBufferBase, size);

		Flush();
	}

	void Renderer2D::Flush()
	{
		RB_PROFILE_FUNC();

		for (uint32_t i = 0; i < sData.textureSlotIndex; i++)
		{
			sData.textureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(sData.vertexArray, sData.quadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec4& color, const glm::vec2& size)
	{
		DrawQuad({ pos.x, pos.y, 0.0f }, color, size);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec4& color, const glm::vec2& size)
	{
		RB_PROFILE_FUNC();

		constexpr float textureIndex = 0; // our default white texture
		constexpr float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		sData.quadVertexBufferPtr->pos = transform * sData.quadVertexPos[0];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->pos = transform * sData.quadVertexPos[1];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->pos = transform * sData.quadVertexPos[2];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->pos = transform * sData.quadVertexPos[3];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadIndexCount += 6;

	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const Ref<Texture2D>& texture, const glm::vec2& size, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ pos.x, pos.y, 0.0f }, texture, size, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const Ref<Texture2D>& texture, const glm::vec2& size, float tilingFactor, const glm::vec4& tintColor)
	{
		RB_PROFILE_FUNC();

		// #TODO: tint color for textures?
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < sData.textureSlotIndex; i++)
		{
			if (*sData.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)sData.textureSlotIndex;
			sData.textureSlots[sData.textureSlotIndex] = texture;
			sData.textureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		sData.quadVertexBufferPtr->pos = transform * sData.quadVertexPos[0];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->pos = transform * sData.quadVertexPos[1];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->pos = transform * sData.quadVertexPos[2];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->pos = transform * sData.quadVertexPos[3];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadIndexCount += 6;

	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec4& color, float angle, const glm::vec2& size /*= {1.0f, 1.0f}*/)
	{
		DrawRotatedQuad({ pos.x, pos.y, 0.0f }, color, angle, size);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec4& color, float angle, const glm::vec2& size /*= {1.0f, 1.0f}*/)
	{
		RB_PROFILE_FUNC();

		constexpr float textureIndex = 0; // default white texture
		constexpr float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), glm::radians(angle), { 0, 0, 1 })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		sData.quadVertexBufferPtr->pos = transform * sData.quadVertexPos[0];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->pos = transform * sData.quadVertexPos[1];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->pos = transform * sData.quadVertexPos[2];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->pos = transform * sData.quadVertexPos[3];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadIndexCount += 6;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const Ref<Texture2D>& texture, float angle, const glm::vec2& size, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ pos.x, pos.y, 0.0f }, texture, angle, size, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const Ref<Texture2D>& texture, float angle, const glm::vec2& size, float tilingFactor, const glm::vec4& tintColor)
	{
		RB_PROFILE_FUNC();
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < sData.textureSlotIndex; i++)
		{
			if (*sData.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)sData.textureSlotIndex;
			sData.textureSlots[sData.textureSlotIndex] = texture;
			sData.textureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), glm::radians(angle), { 0, 0, 1 })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		sData.quadVertexBufferPtr->pos = transform * sData.quadVertexPos[0];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 0.0f, 0.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->pos = transform * sData.quadVertexPos[1];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 1.0f, 0.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->pos = transform * sData.quadVertexPos[2];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 1.0f, 1.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadVertexBufferPtr->pos = transform * sData.quadVertexPos[3];
		sData.quadVertexBufferPtr->color = color;
		sData.quadVertexBufferPtr->texCoord = { 0.0f, 1.0f };
		sData.quadVertexBufferPtr->texIndex = textureIndex;
		sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sData.quadVertexBufferPtr++;

		sData.quadIndexCount += 6;
	}

}

