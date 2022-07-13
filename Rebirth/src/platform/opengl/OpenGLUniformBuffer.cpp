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
// File Name: OpenGLUniformBuffer.cpp
// Date File Created: 7/13/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>


namespace rebirth
{



	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32 size, uint32 binding)
	{
		glCreateBuffers(1, &mId);
		glNamedBufferData(mId, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, mId);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &mId);
	}

	void OpenGLUniformBuffer::SetData(const void* data, uint32 size, uint32 offset /*= 0*/)
	{
		glNamedBufferSubData(mId, offset, size, data);
	}

}
