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
// File Name: OpenGLVertexArray.cpp
// Date File Created: 06/20/2022 at 5:08 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"

#include "OpenGLVertexArray.h"

#include <glad/glad.h>

static GLenum GetGlType(const rebirth::ShaderDataType type)
{
	switch (type)
	{
	case rebirth::ShaderDataType::FLOAT:
	case rebirth::ShaderDataType::FLOAT2:
	case rebirth::ShaderDataType::FLOAT3:
	case rebirth::ShaderDataType::FLOAT4:
	case rebirth::ShaderDataType::MAT3:
	case rebirth::ShaderDataType::MAT4: return GL_FLOAT;
	case rebirth::ShaderDataType::INT:
	case rebirth::ShaderDataType::INT2:
	case rebirth::ShaderDataType::INT3:
	case rebirth::ShaderDataType::INT4: return GL_INT;
	case rebirth::ShaderDataType::BOOL: return GL_BOOL;
	case rebirth::ShaderDataType::NONE: break;
	}

	RB_CORE_ASSERT(false, "Unknown shader data type");
	return 0;
}

namespace rebirth
{

	OpenGLVertexArray::OpenGLVertexArray()
	{
		RB_PROFILE_FUNC();
		glCreateVertexArrays(1, &mId);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		RB_PROFILE_FUNC();
		glDeleteVertexArrays(1, &mId);
	}

	void OpenGLVertexArray::Bind() const
	{
		RB_PROFILE_FUNC();
		glBindVertexArray(mId);
	}

	void OpenGLVertexArray::Unbind() const
	{
		RB_PROFILE_FUNC();
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer)
	{
		RB_PROFILE_FUNC();
		RB_CORE_ASSERT(buffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout");

		glBindVertexArray(mId);
		buffer->Bind();
		const auto& layout = buffer->GetLayout();
		for (const auto& elem : layout)
		{
			switch (elem.type)
			{
				case rebirth::ShaderDataType::FLOAT:
				case rebirth::ShaderDataType::FLOAT2:
				case rebirth::ShaderDataType::FLOAT3:
				case rebirth::ShaderDataType::FLOAT4:
				case rebirth::ShaderDataType::INT:
				case rebirth::ShaderDataType::INT2:
				case rebirth::ShaderDataType::INT3:
				case rebirth::ShaderDataType::INT4:
				case rebirth::ShaderDataType::BOOL:
				{
					glEnableVertexAttribArray(mVertexBufferIndex);
					glVertexAttribPointer(mVertexBufferIndex, elem.GetComponentCount(), GetGlType(elem.type),
						elem.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
						reinterpret_cast<const void*>(static_cast<uintptr_t>(elem.offset)));
					mVertexBufferIndex++;
					break;
				}
				case rebirth::ShaderDataType::MAT3:
				case rebirth::ShaderDataType::MAT4:
				{
					uint8_t count = elem.GetComponentCount();
					for (uint8_t i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(mVertexBufferIndex);
						glVertexAttribPointer(mVertexBufferIndex, count, GetGlType(elem.type),
							elem.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
							reinterpret_cast<const void*>(static_cast<uintptr_t>(sizeof(float) * count * i)));
						glVertexAttribDivisor(mVertexBufferIndex, 1);
						mVertexBufferIndex++;
					}
					break;
				}
				case rebirth::ShaderDataType::NONE:
				default:
					RB_CORE_ASSERT(false, "Unknown shader data type");

			}
		}

		mVertexBuffers.push_back(buffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& buffer)
	{
		RB_PROFILE_FUNC();
		glBindVertexArray(mId);
		buffer->Bind();
		mIndexBuffer = buffer;
	}
}