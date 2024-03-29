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
// File Name: Renderer2D.h
// Date File Created: 6/24/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "OrthoCamera.h"
#include "Camera.h"

#include "Texture.h"
#include "SubTexture.h"
#include "EditorCamera.h"

#include "rebirth/scene/Components.h"

namespace rebirth
{
	class Renderer2D
	{
	public:

		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const OrthoCamera& camera);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		static void Flush();

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityId = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f}, int entityId = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, float tilingFactor = 1.0f, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f}, int entityId = -1);

		static void DrawQuad(const glm::vec2& pos, const glm::vec4& color, const glm::vec2 & size = {1.0f, 1.0f});
		static void DrawQuad(const glm::vec3& pos, const glm::vec4& color, const glm::vec2 & size = {1.0f, 1.0f});

		static void DrawQuad(const glm::vec2& pos, const Ref<Texture2D>& texture, const glm::vec2 & size = {1.0f, 1.0f}, float tilingFactor = 1.0f, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f});
		static void DrawQuad(const glm::vec3& pos, const Ref<Texture2D>& texture, const glm::vec2 & size = {1.0f, 1.0f}, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });

		static void DrawQuad(const glm::vec2& pos, const Ref<SubTexture2D>& subtexture, const glm::vec2& size = { 1.0f, 1.0f }, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& pos, const Ref<SubTexture2D>& subtexture, const glm::vec2& size = { 1.0f, 1.0f }, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });

		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec4& color, float angle, const glm::vec2 & size = {1.0f, 1.0f});
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec4& color, float angle, const glm::vec2 & size = {1.0f, 1.0f});

		static void DrawRotatedQuad(const glm::vec2& pos, const Ref<Texture2D>& texture, float angle, const glm::vec2 & size = {1.0f, 1.0f}, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawRotatedQuad(const glm::vec3& pos, const Ref<Texture2D>& texture, float angle, const glm::vec2 & size = {1.0f, 1.0f}, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });

		static void DrawRotatedQuad(const glm::vec2& pos, const Ref<SubTexture2D>& subtexture, float angle, const glm::vec2& size = { 1.0f, 1.0f }, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawRotatedQuad(const glm::vec3& pos, const Ref<SubTexture2D>& subtexture, float angle, const glm::vec2& size = { 1.0f, 1.0f }, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });


		static void DrawSprite(const glm::mat4& transform, SpriteComponent& spriteComponent, int entityId);

		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityId = -1);

		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, int entityId = -1);

		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityId = -1);
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityId = -1);

		static void SetLineWidth(float width);
		static float GetLineWidth();

		struct Stats
		{
			uint32 drawCalls = 0;
			uint32 quads = 0;

			uint32 GetVertCount() { return quads * 4; }
			uint32 GetIndicesCount() { return quads * 6; }
		};
		static Stats GetStats();
		static void ResetStats();

	private:
		static void StartBatch();
		static void ResetBatch();
		static void CreateQuad(const glm::mat4& transform, const glm::vec4 color, const glm::vec2* texCoord, float texIndex, float tilingFactor, int entityId = -1);
		static float GetTextureIndex(const Ref<Texture2D>& texture);
	};
}

