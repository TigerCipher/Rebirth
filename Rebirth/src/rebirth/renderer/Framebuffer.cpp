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
// File Name: Framebuffer.cpp
// Date File Created: 6/26/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "Framebuffer.h"

#include "Renderer.h"
#include "platform/opengl/OpenGLFramebuffer.h"

namespace rebirth
{

	Ref<Framebuffer> Framebuffer::Create(const FramebufferDesc& spec)
	{
		switch (Renderer::GetAPI())
		{
			case GraphicsAPI::API::NONE:
			{
				RB_CORE_ASSERT(false, "Must use a graphics API");
				return nullptr;
			}

			case GraphicsAPI::API::OPENGL: return createRef<OpenGLFramebuffer>(spec);
		}

		RB_CORE_ASSERT(false, "Unknown graphics API");
		return nullptr;
	}

}