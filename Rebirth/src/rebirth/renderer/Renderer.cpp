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
// File Name: Renderer.cpp
// Date File Created: 06/19/2022 at 4:19 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------


#include "rbpch.h"
#include "Renderer.h"

namespace rebirth
{
	Renderer::Data* Renderer::sData = new Renderer::Data;

	void Renderer::BeginScene(OrthoCamera& camera)
	{
		sData->viewProj = camera.ViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const SharedPtr<Shader>& shader, const SharedPtr<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->SetUniformMat4("uViewProj", sData->viewProj);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}