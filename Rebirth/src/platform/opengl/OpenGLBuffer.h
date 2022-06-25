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
// File Name: OpenGLBuffer.h
// Date File Created: 06/19/2022 at 3:52 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------


#pragma once

#include "rebirth/renderer/Buffer.h"

namespace rebirth
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(uint32_t size, float* vertices);
		~OpenGLVertexBuffer() override;
		void Bind() const override;
		void Unbind() const override;

		void SetLayout(const BufferLayout& layout) override
		{
			mLayout = layout;
		}
		
		const BufferLayout& GetLayout() const override
		{
			return mLayout;
		}

		void SetData(const void* data, uint32_t size) override;
	
	private:
		uint32_t mId;
		BufferLayout mLayout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t count, uint32_t* indices);
		~OpenGLIndexBuffer() override;
		void Bind() const override;
		void Unbind() const override;

		uint32_t GetCount() const override { return mCount; }

	private:
		uint32_t mId;
		uint32_t mCount;
	};
}