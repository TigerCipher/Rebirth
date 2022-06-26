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
// File Name: OpenGLBuffer.cpp
// Date File Created: 06/19/2022 at 4:07 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------


#include "rbpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace rebirth
{

/////////////////////////////////////
/// Vertex Buffer ///////////////////
/////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(const uint32 size)
	{
		RB_PROFILE_FUNC();

		glCreateBuffers(1, &mId);
		glBindBuffer(GL_ARRAY_BUFFER, mId);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const uint32 size, float* vertices)
	{
		RB_PROFILE_FUNC();
		glCreateBuffers(1, &mId);
		glBindBuffer(GL_ARRAY_BUFFER, mId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}


	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		RB_PROFILE_FUNC();
		glDeleteBuffers(1, &mId);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		RB_PROFILE_FUNC();
		glBindBuffer(GL_ARRAY_BUFFER, mId);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		RB_PROFILE_FUNC();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	void OpenGLVertexBuffer::SetData(const void* data, uint32 size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	/////////////////////////////////////
	/// Index Buffer ////////////////////
	/////////////////////////////////////


	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32 count, uint32* indices) :
		mCount(count)
	{
		RB_PROFILE_FUNC();
		glCreateBuffers(1, &mId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		RB_PROFILE_FUNC();
		glDeleteBuffers(1, &mId);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		RB_PROFILE_FUNC();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		RB_PROFILE_FUNC();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}