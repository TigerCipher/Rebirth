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
// File Name: UniformBuffer.cpp
// Date File Created: 7/13/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "UniformBuffer.h"

#include "Renderer.h"
#include "platform/opengl/OpenGLUniformBuffer.h"


namespace rebirth
{

	Ref<UniformBuffer> UniformBuffer::Create(uint32 size, uint32 binding)
	{
		switch (Renderer::GetAPI())
		{
			case GraphicsAPI::API::NONE:
				RB_CORE_ASSERT(false, "Must use a graphics API");
				return nullptr;

			case GraphicsAPI::API::OPENGL: return createRef<OpenGLUniformBuffer>(size, binding);
		}

		RB_CORE_ASSERT(false, "Unknown graphics API");
		return nullptr;
	}

}

