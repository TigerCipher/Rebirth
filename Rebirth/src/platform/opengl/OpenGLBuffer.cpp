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

/////////////////////////////////////
/// Vertex Buffer ///////////////////
/////////////////////////////////////


rebirth::OpenGLVertexBuffer::OpenGLVertexBuffer(const uint32_t size, float* vertices)
{
	glCreateBuffers(1, &mID);
	glBindBuffer(GL_ARRAY_BUFFER, mID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

rebirth::OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1, &mID);
}

void rebirth::OpenGLVertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, mID);
}

void rebirth::OpenGLVertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/////////////////////////////////////
/// Index Buffer ////////////////////
/////////////////////////////////////


rebirth::OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t count, uint32_t* indices) :
	mCount(count)
{
	glCreateBuffers(1, &mID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

rebirth::OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	glDeleteBuffers(1, &mID);
}

void rebirth::OpenGLIndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
}

void rebirth::OpenGLIndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
