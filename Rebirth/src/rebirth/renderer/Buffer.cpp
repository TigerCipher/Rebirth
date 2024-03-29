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
// File Name: Buffer.cpp
// Date File Created: 06/19/2022 at 3:46 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "platform/opengl/OpenGLBuffer.h"

namespace rebirth
{

	Ref<rebirth::VertexBuffer> VertexBuffer::Create(uint32 size)
	{
		switch (Renderer::GetAPI())
		{
			case GraphicsAPI::API::NONE:
			{
				RB_CORE_ASSERT(false, "Must use a graphics API");
				return nullptr;
			}

			case GraphicsAPI::API::OPENGL: return createRef<OpenGLVertexBuffer>(size);
		}

		RB_CORE_ASSERT(false, "Unknown graphics API");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(const uint32 size, float* vertices)
	{
		switch (Renderer::GetAPI())
		{
		case GraphicsAPI::API::NONE:
		{
			RB_CORE_ASSERT(false, "Must use a graphics API");
			return nullptr;
		}

		case GraphicsAPI::API::OPENGL: return createRef<OpenGLVertexBuffer>(size, vertices);
		}

		RB_CORE_ASSERT(false, "Unknown graphics API");
		return nullptr;
	}



	Ref<IndexBuffer> IndexBuffer::Create(const uint32 count, uint32* indices)
	{
		switch (Renderer::GetAPI())
		{
		case GraphicsAPI::API::NONE:
		{
			RB_CORE_ASSERT(false, "Must use a graphics API");
			return nullptr;
		}

		case GraphicsAPI::API::OPENGL:
		{
			return createRef<OpenGLIndexBuffer>(count, indices);
		}
		}

		RB_CORE_ASSERT(false, "Unknown graphics API");
		return nullptr;
	}
}