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
// File Name: Renderer.h
// Date File Created: 06/19/2022 at 4:18 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------


#pragma once

#include "RenderCommand.h"

#include "OrthoCamera.h"
#include "Shader.h"

namespace rebirth
{


	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32 width, uint32 height);

		static void BeginScene(OrthoCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static GraphicsAPI::API GetAPI() { return GraphicsAPI::GetAPI(); }

	private:
		struct Data
		{
			glm::mat4 viewProj;
		};

		static Scope<Data> sData;
	};
}