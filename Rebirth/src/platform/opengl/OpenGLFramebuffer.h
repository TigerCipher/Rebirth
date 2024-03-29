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
// File Name: OpenGLFramebuffer.h
// Date File Created: 6/26/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "rebirth/renderer/Framebuffer.h"

namespace rebirth
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferDesc& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();
		void Bind() override;
		void Unbind() override;


		uint32 GetColorAttachmentID(uint32 index = 0) const override { RB_CORE_ASSERT(index < mColorAttachments.size()); return mColorAttachments[index]; }
		const FramebufferDesc& GetDesc() const override
		{
			return mDesc;
		}


		void Resize(uint32 width, uint32 height) override;


		int ReadPixel(uint32 attachmentIndex, int x, int y) override;


		void ClearAttachment(uint32 attachmentIndex, int value) override;

	private:
		FramebufferDesc mDesc;
		uint32 mId = 0;

		std::vector<FramebufferTextureDesc> mColorAttachmentDesc;
		FramebufferTextureDesc mDepthAttachmentDesc;

		std::vector<uint32> mColorAttachments;
		uint32 mDepthAttachment = 0;
	};
}
