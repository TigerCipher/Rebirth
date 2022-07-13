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
// File Name: Framebuffer.h
// Date File Created: 6/26/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once


namespace rebirth
{

	enum class FramebufferTextureFormat
	{
		NONE = 0,

		RGBA8,
		RED_INT,

		DEPTH24_STENCIL8,

		DEPTH = DEPTH24_STENCIL8
	};

	struct FramebufferTextureDesc
	{
		FramebufferTextureDesc() = default;
		FramebufferTextureDesc(FramebufferTextureFormat format) : textureFormat(format) {}

		FramebufferTextureFormat textureFormat = FramebufferTextureFormat::NONE;
	};

	struct FramebufferAttachmentDesc
	{
		FramebufferAttachmentDesc() = default;
		FramebufferAttachmentDesc(std::initializer_list<FramebufferTextureDesc> pAttachments) : attachments(pAttachments) {}
		std::vector<FramebufferTextureDesc> attachments;
	};

	struct FramebufferDesc
	{
		uint32 width, height;
		uint32 samples = 1;

		FramebufferAttachmentDesc attachements;

		// #FUTURE: Will likely want this for Vulkan, maybe directx - not opengl though I don't think
		//bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:

		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32 width, uint32 height) = 0;
		virtual int ReadPixel(uint32 attachmentIndex, int x, int y) = 0;

		virtual uint32 GetColorAttachmentID(uint32 index = 0) const = 0;
		virtual const FramebufferDesc& GetDesc() const = 0;

		static Ref<Framebuffer> Create(const FramebufferDesc& spec);
	};
}
