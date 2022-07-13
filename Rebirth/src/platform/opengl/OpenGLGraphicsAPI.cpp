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
// File Name: OpenGLGraphicsAPI.cpp
// Date File Created: 6/21/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"

#include "OpenGLGraphicsAPI.h"

#include <glad/glad.h>

namespace rebirth
{


	void OpenGLGraphicsAPI::Init()
	{
		RB_PROFILE_FUNC();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);

		auto& capabilities = GraphicsAPI::GetCapabilities();
		capabilities.version = (const char*)glGetString(GL_VERSION);
		capabilities.renderer = (const char*)glGetString(GL_RENDERER);
		capabilities.vendor = (const char*)glGetString(GL_VENDOR);

		glGetIntegerv(GL_MAX_SAMPLES, &capabilities.maxSamples);
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &capabilities.maxAniostropy);
	}

	void OpenGLGraphicsAPI::SetViewport(const uint32 x, const uint32 y, const uint32 width, const uint32 height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLGraphicsAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLGraphicsAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLGraphicsAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32 count)
	{
		uint32 indexCount = count ? vertexArray->GetIndexBuffer()->GetCount() : count;
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}
}