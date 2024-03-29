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
#include "UniformBuffer.h"

namespace rebirth
{

	struct QuadVertex
	{
		glm::vec3 pos;
		glm::vec4 color;
		glm::vec2 texCoord;
		float texIndex;
		float tilingFactor;

		// editor
		int entityId = -1;
	};

	struct CircleVertex
	{
		glm::vec3 worldPos;
		glm::vec3 localPos;
		glm::vec4 color;
		float thickness;
		float fade;

		// editor
		int entityId = -1;
	};

	struct LineVertex
	{
		glm::vec3 position;
		glm::vec4 color;

		// editor
		int entityId = -1;
	};

	struct RenderData
	{
		static const uint32 MAX_QUADS = 10000;
		static const uint32 MAX_VERTS = MAX_QUADS * 4;
		static const uint32 MAX_INDICES = MAX_QUADS * 6;
		static const uint32 MAX_TEXTURE_SLOTS = 32;

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> quadShader;

		Ref<VertexArray> circleVertexArray;
		Ref<VertexBuffer> circleVertexBuffer;
		Ref<Shader> circleShader;

		Ref<VertexArray> lineVertexArray;
		Ref<VertexBuffer> lineVertexBuffer;
		Ref<Shader> lineShader;


		uint32 quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		uint32 circleIndexCount = 0;
		CircleVertex* circleVertexBufferBase = nullptr;
		CircleVertex* circleVertexBufferPtr = nullptr;

		uint32 lineVertexCount = 0;
		LineVertex* lineVertexBufferBase = nullptr;
		LineVertex* lineVertexBufferPtr = nullptr;

		float lineWidth = 2.0f;

		glm::vec4 quadVertexPos[4];

		Ref<Texture2D> whiteTexture;
		std::array<Ref<Texture2D>, MAX_TEXTURE_SLOTS> textureSlots;
		uint32 textureSlotIndex = 1; // we use 0 for our default white texture



		Renderer2D::Stats stats;

		struct CameraData
		{
			glm::mat4 viewProjection;
		};

		CameraData cameraBuffer;
		Ref<UniformBuffer> cameraUniformBuffer;
	};

	static RenderData sData;



	void Renderer2D::Init()
	{
		RB_PROFILE_FUNC();

		// Quads
		sData.quadVertexArray = VertexArray::Create();

		sData.quadVertexBuffer = VertexBuffer::Create(sData.MAX_VERTS * sizeof(QuadVertex));
		sData.quadVertexBuffer->SetLayout({
				{ ShaderDataType::FLOAT3, "aPos" },
				{ ShaderDataType::FLOAT4, "aColor" },
				{ ShaderDataType::FLOAT2, "aTexCoord" },
				{ ShaderDataType::FLOAT, "aTexIndex" },
				{ ShaderDataType::FLOAT, "aTilingFactor" },
				{ ShaderDataType::INT, "aEntityID" },
			});
		sData.quadVertexArray->AddVertexBuffer(sData.quadVertexBuffer);

		sData.quadVertexBufferBase = new QuadVertex[sData.MAX_VERTS];

		uint32* quadIndices = new uint32[sData.MAX_INDICES];

		uint32 offset = 0;
		for (uint32 i = 0; i < sData.MAX_INDICES; i += 6)
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
		sData.quadVertexArray->SetIndexBuffer(sib);
		delete[] quadIndices;


		// Circles
		sData.circleVertexArray = VertexArray::Create();

		sData.circleVertexBuffer = VertexBuffer::Create(sData.MAX_VERTS * sizeof(CircleVertex));
		sData.circleVertexBuffer->SetLayout({
				{ ShaderDataType::FLOAT3, "aWorldPos" },
				{ ShaderDataType::FLOAT3, "aLocalPos" },
				{ ShaderDataType::FLOAT4, "aColor" },
				{ ShaderDataType::FLOAT, "aThickness" },
				{ ShaderDataType::FLOAT, "aFade" },
				{ ShaderDataType::INT, "aEntityID" },
			});
		sData.circleVertexArray->AddVertexBuffer(sData.circleVertexBuffer);
		sData.circleVertexArray->SetIndexBuffer(sib);
		sData.circleVertexBufferBase = new CircleVertex[sData.MAX_VERTS];


		// Lines
		sData.lineVertexArray = VertexArray::Create();

		sData.lineVertexBuffer = VertexBuffer::Create(sData.MAX_VERTS * sizeof(LineVertex));
		sData.lineVertexBuffer->SetLayout({
				{ ShaderDataType::FLOAT3, "aPos" },
				{ ShaderDataType::FLOAT4, "aColor" },
				{ ShaderDataType::INT, "aEntityID" },
			});
		sData.lineVertexArray->AddVertexBuffer(sData.lineVertexBuffer);
		sData.lineVertexBufferBase = new LineVertex[sData.MAX_VERTS];



		sData.quadShader = Shader::Create("assets/shaders/Quad.glsl");
		sData.circleShader = Shader::Create("assets/shaders/Circle.glsl");
		sData.lineShader = Shader::Create("assets/shaders/Line.glsl");


		sData.whiteTexture = Texture2D::Create(1, 1);
		uint32 whiteData = 0xffffffff;
		sData.whiteTexture->SetData(&whiteData, sizeof(uint32));

		int32_t samplers[sData.MAX_TEXTURE_SLOTS];
		for (uint32 i = 0; i < sData.MAX_TEXTURE_SLOTS; i++)
		{
			samplers[i] = i;
		}

		sData.textureSlots[0] = sData.whiteTexture;

		sData.quadVertexPos[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		sData.quadVertexPos[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		sData.quadVertexPos[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		sData.quadVertexPos[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		sData.cameraUniformBuffer = UniformBuffer::Create(sizeof(RenderData::CameraData), 0);
	}

	void Renderer2D::Shutdown()
	{
		//RB_PROFILE_FUNC();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		RB_PROFILE_FUNC();
		sData.cameraBuffer.viewProjection = camera.GetProjection() * glm::inverse(transform);
		sData.cameraUniformBuffer->SetData(&sData.cameraBuffer, sizeof(RenderData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const OrthoCamera& camera)
	{
		RB_PROFILE_FUNC();
		sData.cameraBuffer.viewProjection = camera.ViewProjectionMatrix();
		sData.cameraUniformBuffer->SetData(&sData.cameraBuffer, sizeof(RenderData::CameraData));

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		RB_PROFILE_FUNC();
		sData.cameraBuffer.viewProjection = camera.GetViewProjection();
		sData.cameraUniformBuffer->SetData(&sData.cameraBuffer, sizeof(RenderData::CameraData));

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		RB_PROFILE_FUNC();


		Flush();
	}

	void Renderer2D::Flush()
	{
		RB_PROFILE_FUNC();
		if (sData.quadIndexCount)
		{
			uint32 size = (uint32)((uint8_t*)sData.quadVertexBufferPtr - (uint8_t*)sData.quadVertexBufferBase);
			sData.quadVertexBuffer->SetData(sData.quadVertexBufferBase, size);

			for (uint32 i = 0; i < sData.textureSlotIndex; i++)
			{
				sData.textureSlots[i]->Bind(i);
			}
			sData.quadShader->Bind();
			RenderCommand::DrawIndexed(sData.quadVertexArray, sData.quadIndexCount);
			sData.stats.drawCalls++;
		}

		if (sData.circleIndexCount)
		{
			uint32 size = (uint32)((uint8_t*)sData.circleVertexBufferPtr - (uint8_t*)sData.circleVertexBufferBase);
			sData.circleVertexBuffer->SetData(sData.circleVertexBufferBase, size);

			sData.circleShader->Bind();
			RenderCommand::DrawIndexed(sData.circleVertexArray, sData.circleIndexCount);
			sData.stats.drawCalls++;
		}

		if (sData.lineVertexCount)
		{
			uint32 size = (uint32)((uint8_t*)sData.lineVertexBufferPtr - (uint8_t*)sData.lineVertexBufferBase);
			sData.lineVertexBuffer->SetData(sData.lineVertexBufferBase, size);

			sData.lineShader->Bind();
			RenderCommand::SetLineWidth(sData.lineWidth);
			RenderCommand::DrawLines(sData.lineVertexArray, sData.lineVertexCount);
			sData.stats.drawCalls++;
		}

	}

	void Renderer2D::StartBatch()
	{
		sData.quadIndexCount = 0;
		sData.quadVertexBufferPtr = sData.quadVertexBufferBase;

		sData.circleIndexCount = 0;
		sData.circleVertexBufferPtr = sData.circleVertexBufferBase;

		sData.lineVertexCount = 0;
		sData.lineVertexBufferPtr = sData.lineVertexBufferBase;

		sData.textureSlotIndex = 1;
	}

	void Renderer2D::ResetBatch()
	{
		Flush();
		StartBatch();
	}


	void Renderer2D::CreateQuad(const glm::mat4& transform, const glm::vec4 color, const glm::vec2* texCoord, float texIndex, float tilingFactor, int entityId)
	{
		constexpr size_t quadVertexCount = 4;
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			sData.quadVertexBufferPtr->pos = transform * sData.quadVertexPos[i];
			sData.quadVertexBufferPtr->color = color;
			sData.quadVertexBufferPtr->texCoord = texCoord[i];
			sData.quadVertexBufferPtr->texIndex = texIndex;
			sData.quadVertexBufferPtr->tilingFactor = tilingFactor;
			sData.quadVertexBufferPtr->entityId = entityId;
			sData.quadVertexBufferPtr++;
		}


		sData.quadIndexCount += 6;
		sData.stats.quads++;
	}

	float Renderer2D::GetTextureIndex(const Ref<Texture2D>& texture)
	{
		float textureIndex = 0.0f;
		for (uint32 i = 1; i < sData.textureSlotIndex; i++)
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

		return textureIndex;
	}

	// COLOR DRAWQUAD

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityId)
	{
		RB_PROFILE_FUNC();

		if (sData.quadIndexCount >= RenderData::MAX_INDICES)
		{
			ResetBatch();
		}

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		CreateQuad(transform, color, textureCoords, 0, 1.0f, entityId);
	}

	// TEXTURE DRAWQUAD

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor, int entityId)
	{
		RB_PROFILE_FUNC();

		if (sData.quadIndexCount >= RenderData::MAX_INDICES)
		{
			ResetBatch();
		}

		float textureIndex = GetTextureIndex(texture);

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		CreateQuad(transform, tintColor, textureCoords, textureIndex, tilingFactor, entityId);
	}

	// SUBTEXTURE DRAWQUAD

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor, int entityId)
	{
		RB_PROFILE_FUNC();

		if (sData.quadIndexCount >= RenderData::MAX_INDICES)
		{
			ResetBatch();
		}

		float textureIndex = GetTextureIndex(subtexture->GetTexture());

		const glm::vec2* textureCoords = subtexture->GetTexCoords();

		CreateQuad(transform, tintColor, textureCoords, textureIndex, tilingFactor, entityId);
	}



	////////////////////////////////////////////////////////////////////////////
	///////////////// DrawQuads below call ones above //////////////////////////
	////////////////////////////////////////////////////////////////////////////





	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec4& color, const glm::vec2& size)
	{
		DrawQuad({ pos.x, pos.y, 0.0f }, color, size);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec4& color, const glm::vec2& size)
	{

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);

	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const Ref<Texture2D>& texture, const glm::vec2& size, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ pos.x, pos.y, 0.0f }, texture, size, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const Ref<Texture2D>& texture, const glm::vec2& size, float tilingFactor, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}


	void Renderer2D::DrawQuad(const glm::vec2& pos, const Ref<SubTexture2D>& subtexture, const glm::vec2& size, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawQuad({ pos.x, pos.y, 0.0f }, subtexture, size, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const Ref<SubTexture2D>& subtexture, const glm::vec2& size, float tilingFactor, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subtexture, tilingFactor, tintColor);

	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec4& color, float angle, const glm::vec2& size /*= {1.0f, 1.0f}*/)
	{
		DrawRotatedQuad({ pos.x, pos.y, 0.0f }, color, angle, size);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec4& color, float angle, const glm::vec2& size /*= {1.0f, 1.0f}*/)
	{
		RB_PROFILE_FUNC();

		if (sData.quadIndexCount >= RenderData::MAX_INDICES)
		{
			ResetBatch();
		}

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), angle, { 0, 0, 1 })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		CreateQuad(transform, color, textureCoords, 0, 1.0f);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const Ref<Texture2D>& texture, float angle, const glm::vec2& size, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ pos.x, pos.y, 0.0f }, texture, angle, size, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const Ref<Texture2D>& texture, float angle, const glm::vec2& size, float tilingFactor, const glm::vec4& tintColor)
	{
		RB_PROFILE_FUNC();

		if (sData.quadIndexCount >= RenderData::MAX_INDICES)
		{
			ResetBatch();
		}

		float textureIndex = GetTextureIndex(texture);

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), angle, { 0, 0, 1 })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		CreateQuad(transform, tintColor, textureCoords, textureIndex, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const Ref<SubTexture2D>& subtexture, float angle, const glm::vec2& size, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ pos.x, pos.y, 0.0f }, subtexture, angle, size, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const Ref<SubTexture2D>& subtexture, float angle, const glm::vec2& size, float tilingFactor, const glm::vec4& tintColor)
	{
		RB_PROFILE_FUNC();

		if (sData.quadIndexCount >= RenderData::MAX_INDICES)
		{
			ResetBatch();
		}


		float textureIndex = GetTextureIndex(subtexture->GetTexture());

		const glm::vec2* textureCoords = subtexture->GetTexCoords();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
			* glm::rotate(glm::mat4(1.0f), angle, { 0, 0, 1 })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		CreateQuad(transform, tintColor, textureCoords, textureIndex, tilingFactor);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteComponent& spriteComponent, int entityId)
	{
		if (spriteComponent.texture)
			DrawQuad(transform, spriteComponent.texture, spriteComponent.tilingFactor, spriteComponent.color, entityId);
		else
			DrawQuad(transform, spriteComponent.color, entityId);
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness /*= 1.0f*/, float fade /*= 0.005f*/, int entityId /*= -1*/)
	{
		RB_PROFILE_FUNC();

		// #TODO Resetting batch for circles

		for (size_t i = 0; i < 4; i++)
		{
			sData.circleVertexBufferPtr->worldPos = transform * sData.quadVertexPos[i];
			sData.circleVertexBufferPtr->localPos = sData.quadVertexPos[i] * 2.0f;
			sData.circleVertexBufferPtr->color = color;
			sData.circleVertexBufferPtr->thickness = thickness;
			sData.circleVertexBufferPtr->fade = fade;
			sData.circleVertexBufferPtr->entityId = entityId;
			sData.circleVertexBufferPtr++;
		}


		sData.circleIndexCount += 6;
		sData.stats.quads++;
	}

	void Renderer2D::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, int entityId /*= -1*/)
	{
		sData.lineVertexBufferPtr->position = start;
		sData.lineVertexBufferPtr->color = color;
		sData.lineVertexBufferPtr->entityId = entityId;
		sData.lineVertexBufferPtr++;

		sData.lineVertexBufferPtr->position = end;
		sData.lineVertexBufferPtr->color = color;
		sData.lineVertexBufferPtr->entityId = entityId;
		sData.lineVertexBufferPtr++;

		sData.lineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityId /*= -1*/)
	{
		glm::vec3 bottomLeft = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 bottomRight = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 topRight = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 topLeft = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(bottomLeft, bottomRight, color);	// bottom edge
		DrawLine(bottomRight, topRight, color);		// right edge
		DrawLine(topRight, topLeft, color);			// top edge
		DrawLine(topLeft, bottomLeft, color);		// left edge
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityId /*= -1*/)
	{
		glm::vec3 lineVerts[4];
		for (int i = 0; i < 4; i++)
		{
			lineVerts[i] = transform * sData.quadVertexPos[i];
		}

		DrawLine(lineVerts[0], lineVerts[1], color);	// bottom edge
		DrawLine(lineVerts[1], lineVerts[2], color);	// right edge
		DrawLine(lineVerts[2], lineVerts[3], color);	// top edge
		DrawLine(lineVerts[3], lineVerts[0], color);    // left edge
	}

	void Renderer2D::SetLineWidth(float width)
	{
		sData.lineWidth = width;
	}

	float Renderer2D::GetLineWidth()
	{
		return sData.lineWidth;
	}

	Renderer2D::Stats Renderer2D::GetStats()
	{
		return sData.stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&sData.stats, 0, sizeof(Stats));
	}



}

