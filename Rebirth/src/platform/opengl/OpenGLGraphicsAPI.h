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
// File Name: OpenGLRendererAPI.h
// Date File Created: 6/21/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once
#include "rebirth/renderer/GraphicsAPI.h"

namespace rebirth
{
	class OpenGLGraphicsAPI : public GraphicsAPI
	{
	public:

		void Init() override;
		void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;
		void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32 count = 0) override;
	};
}
