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
	class RendererAPI
	{
	public:
		enum class API
		{
			NONE = 0,
			OPENGL
		};

		virtual void Init() = 0;

		virtual void SetViewport(uint x, uint y, uint width, uint height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		static API GetAPI() { return sApi;  }

	private:
		static API sApi;
	};
}

