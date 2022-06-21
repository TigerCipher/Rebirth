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
// File Name: OpenGLVertexArray.h
// Date File Created: 06/20/2022 at 5:08 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once

#include "rebirth/renderer/VertexArray.h"

namespace rebirth
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() override;
		
		void Bind() const override;
		void Unbind() const override;

		void AddVertexBuffer(const SharedPtr<VertexBuffer>& buffer) override;
		void SetIndexBuffer(const SharedPtr<IndexBuffer>& buffer) override;

		const std::vector<SharedPtr<VertexBuffer>>& GetVertexBuffers() const override
		{
			return mVertexBuffers;
		}

		const SharedPtr<IndexBuffer>& GetIndexBuffer() const override
		{
			return mIndexBuffer;
		}
	
	private:
		uint32_t mId = 0;
		std::vector<SharedPtr<VertexBuffer>> mVertexBuffers;
		SharedPtr<IndexBuffer> mIndexBuffer;
	};
}

