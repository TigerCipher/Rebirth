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

#include "Texture.h"

namespace rebirth
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthoCamera& camera);
		static void EndScene();

		static void Flush();

		static void DrawQuad(const glm::vec2& pos, const glm::vec4& color, const glm::vec2 & size = {1.0f, 1.0f});
		static void DrawQuad(const glm::vec3& pos, const glm::vec4& color, const glm::vec2 & size = {1.0f, 1.0f});

		static void DrawQuad(const glm::vec2& pos, const Ref<Texture2D>& texture, const glm::vec2 & size = {1.0f, 1.0f}, float tilingFactor = 1.0f, const glm::vec4& tintColor = {1.0f, 1.0f, 1.0f, 1.0f});
		static void DrawQuad(const glm::vec3& pos, const Ref<Texture2D>& texture, const glm::vec2 & size = {1.0f, 1.0f}, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });

		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec4& color, float angle, const glm::vec2 & size = {1.0f, 1.0f});
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec4& color, float angle, const glm::vec2 & size = {1.0f, 1.0f});
		static void DrawRotatedQuad(const glm::vec2& pos, const Ref<Texture2D>& texture, float angle, const glm::vec2 & size = {1.0f, 1.0f}, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawRotatedQuad(const glm::vec3& pos, const Ref<Texture2D>& texture, float angle, const glm::vec2 & size = {1.0f, 1.0f}, float tilingFactor = 1.0f, const glm::vec4& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f });


		struct Stats
		{
			uint drawCalls = 0;
			uint quads = 0;

			uint GetVertCount() { return quads * 4; }
			uint GetIndicesCount() { return quads * 6; }
		};
		static Stats GetStats();
		static void ResetStats();

	private:
		static void ResetBatch();

	};
}

