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
// File Name: RendererAPI.h
// Date File Created: 6/21/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once



#include "VertexArray.h"

namespace rebirth
{
	class GraphicsAPI
	{
	public:
		enum class API
		{
			NONE = 0,
			OPENGL
		};

		struct RenderCapabilities
		{
			std::string version;
			std::string renderer;
			std::string vendor;
			int maxSamples;
			float maxAniostropy;
		};

		virtual ~GraphicsAPI() = default;

		virtual void Init() = 0;

		virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32 indexCount = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32 vertexCount) = 0;

		virtual void SetLineWidth(float width) = 0;

		static API GetAPI() { return sApi;  }

		static RenderCapabilities& GetCapabilities()
		{
			static RenderCapabilities capabilities;
			return capabilities;
		}

	private:
		static API sApi;
	};
}

