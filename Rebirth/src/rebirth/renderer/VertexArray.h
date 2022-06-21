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

#pragma once

#include "Buffer.h"

namespace rebirth
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const SharedPtr<VertexBuffer>& buffer) = 0;
		virtual void SetIndexBuffer(const SharedPtr<IndexBuffer>& buffer) = 0;

		virtual const std::vector<SharedPtr<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const SharedPtr<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}