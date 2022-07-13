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
// File Name: RenderCommand.h
// Date File Created: 6/21/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "GraphicsAPI.h"

namespace rebirth
{
	class RenderCommand
	{
	public:

		inline static void Init()
		{
			sRendererApi->Init();
		}

		inline static void SetViewport(const uint32 x, const uint32 y, const uint32 width, const uint32 height)
		{
			sRendererApi->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			sRendererApi->SetClearColor(color);
		}

		inline static void Clear()
		{
			sRendererApi->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32 count = 0)
		{
			sRendererApi->DrawIndexed(vertexArray, count);
		}

	private:
		static Scope<GraphicsAPI> sRendererApi;
	};

}

