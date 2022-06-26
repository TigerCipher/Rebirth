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
// File Name: Buffer.h
// Date File Created: 06/19/2022 at 3:44 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once


namespace rebirth
{
	enum class ShaderDataType
	{
		NONE = 0,
		FLOAT,
		FLOAT2,
		FLOAT3,
		FLOAT4,
		MAT3,
		MAT4,
		INT,
		INT2,
		INT3,
		INT4,
		BOOL
	};

	static uint32 ShaderDataTypeSize(const ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::FLOAT: return 4;
			case ShaderDataType::FLOAT2: return 4 * 2;
			case ShaderDataType::FLOAT3: return 4 * 3;
			case ShaderDataType::FLOAT4: return 4 * 4;
			case ShaderDataType::MAT3: return 4 * 3 * 3;
			case ShaderDataType::MAT4: return 4 * 4 * 4;
			case ShaderDataType::INT: return 4;
			case ShaderDataType::INT2: return 4 * 2;
			case ShaderDataType::INT3: return 4 * 3;
			case ShaderDataType::INT4: return 4 * 4;
			case ShaderDataType::BOOL: return 1;
			case ShaderDataType::NONE:
				RB_CORE_ASSERT(false, "Unknown shader data type");
				return 0;
		}

		RB_CORE_ASSERT(false, "Unknown shader data type");
		return 0;
	}

	struct BufferElement
	{
		std::string name;
		ShaderDataType type;
		uint32 size;
		uint32 offset;
		bool normalized;

		BufferElement() = default;
		
		BufferElement(const ShaderDataType pType, const std::string& pName) :
			name(pName), type(pType), size(ShaderDataTypeSize(pType)), offset(0), normalized(false) {}

		uint32 GetComponentCount() const
		{
			switch(type)
			{
			case ShaderDataType::FLOAT: return 1;
			case ShaderDataType::FLOAT2: return 2;
			case ShaderDataType::FLOAT3: return 3;
			case ShaderDataType::FLOAT4: return 4;
			case ShaderDataType::MAT3: return 3;// *3;
			case ShaderDataType::MAT4: return 4;// *4;
			case ShaderDataType::INT: return 1;
			case ShaderDataType::INT2: return 2;
			case ShaderDataType::INT3: return 3;
			case ShaderDataType::INT4: return 4;
			case ShaderDataType::BOOL: return 1;
			case ShaderDataType::NONE:
				{
					RB_CORE_ASSERT(false, "Unknown shader data type");
					return 0;
				}
			}

			RB_CORE_ASSERT(false, "Unknown shader data type");
			return 0;
		}
		
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements) : mElements(elements)
		{
			CalculateOffsetsAndStride();
		}
		
		const std::vector<BufferElement>& GetElements() const { return mElements; }

		uint32 GetStride() const { return mStride; }

		
		std::vector<BufferElement>::iterator begin() { return mElements.begin(); }
		std::vector<BufferElement>::iterator end() { return mElements.end(); }

		std::vector<BufferElement>::const_iterator begin() const { return mElements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return mElements.end(); }
	
	private:
		std::vector<BufferElement> mElements;
		uint32 mStride = 0;

		void CalculateOffsetsAndStride()
		{
			uint32 offset = 0;
			mStride = 0;
			for(auto& elem : mElements)
			{
				elem.offset = offset;
				offset += elem.size;
				mStride += elem.size;
			}
		}
	};


	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		virtual void SetData(const void* data, uint32 size) = 0;

		static Ref<VertexBuffer> Create(uint32 size);
		static Ref<VertexBuffer> Create(uint32 size, float* vertices);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32 GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32 count, uint32* indices);
	};
}
