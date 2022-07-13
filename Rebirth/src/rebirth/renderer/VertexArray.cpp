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
// File Name: VertexArray.h
// Date File Created: 06/20/2022 at 4:51 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"

#include "VertexArray.h"

#include "Renderer.h"
#include "platform/opengl/OpenGLVertexArray.h"

namespace rebirth
{

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case GraphicsAPI::API::NONE:
			{
				RB_CORE_ASSERT(false, "Must use a graphics API");
				return nullptr;
			}

			case GraphicsAPI::API::OPENGL: return createRef<OpenGLVertexArray>();
		}

		RB_CORE_ASSERT(false, "Unknown graphics API");
		return nullptr;
	}
}