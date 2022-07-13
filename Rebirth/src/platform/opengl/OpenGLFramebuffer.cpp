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
// File Name: OpenGLFramebuffer.cpp
// Date File Created: 6/26/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace rebirth
{

	static constexpr uint32 sMaxFramebufferSize = 8192;

	static bool IsDepthFormat(FramebufferTextureFormat format)
	{
		switch (format)
		{
			case FramebufferTextureFormat::DEPTH24_STENCIL8: return true;
		}
		return false;
	}

	static GLenum TextureTarget(bool multisamples)
	{
		return multisamples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void CreateTextures(bool multisamples, uint32* outId, uint32 count)
	{
		glCreateTextures(TextureTarget(multisamples), count, outId);
	}

	static void BindTexture(bool multisamples, uint32 id)
	{
		glBindTexture(TextureTarget(multisamples), id);
	}

	static void AttachColorTexture(uint32 id, int samples, GLenum internalFormat, GLenum format, uint32 width, uint32 height, int index)
	{
		bool multisamples = samples > 1;
		if (multisamples)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisamples), id, 0);
	}

	static void AttachDepthTexture(uint32 id, int samples, GLenum format, GLenum type, int width, int height)
	{
		bool multisamples = samples > 1;
		if (multisamples)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, type, TextureTarget(multisamples), id, 0);
	}

	static GLenum GetGlFormat(FramebufferTextureFormat format)
	{
		switch (format)
		{
			case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
			case FramebufferTextureFormat::RED_INT: return GL_RED_INTEGER;
		}

		RB_CORE_ASSERT(false);
		return 0;
	}

	//static GLenum GetGlTypeFromFormat(FramebufferTextureFormat format)
	//{
	//	switch (format)
	//	{
	//		case FramebufferTextureFormat::RGBA8: return GL_UNSIGNED_BYTE;
	//		case FramebufferTextureFormat::RED_INT: return GL_INT;
	//	}

	//	RB_CORE_ASSERT(false);
	//	return 0;
	//}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferDesc& spec) :
		mDesc(spec)
	{
		for (auto format : mDesc.attachements.attachments)
		{
			if (!IsDepthFormat(format.textureFormat))
				mColorAttachmentDesc.emplace_back(format);
			else
				mDepthAttachmentDesc = format;
		}
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &mId);
		glDeleteTextures((GLsizei)mColorAttachments.size(), mColorAttachments.data());
		glDeleteTextures(1, &mDepthAttachment);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		RB_PROFILE_FUNC();

		if(mId)
		{
			glDeleteFramebuffers(1, &mId);
			glDeleteTextures((GLsizei)mColorAttachments.size(), mColorAttachments.data());
			glDeleteTextures(1, &mDepthAttachment);
			mColorAttachments.clear();
			mDepthAttachment = 0;
		}

		glCreateFramebuffers(1, &mId);
		glBindFramebuffer(GL_FRAMEBUFFER, mId);


		// Attachments

		bool multisamples = mDesc.samples > 1;
		if (mColorAttachmentDesc.size())
		{
			mColorAttachments.resize(mColorAttachmentDesc.size());
			CreateTextures(multisamples, mColorAttachments.data(), (uint32)mColorAttachments.size());
			for (uint32 i = 0; i < mColorAttachments.size(); i++)
			{
				BindTexture(multisamples, mColorAttachments[i]);
				switch (mColorAttachmentDesc[i].textureFormat)
				{
					case FramebufferTextureFormat::RGBA8:
						AttachColorTexture(mColorAttachments[i], mDesc.samples, GL_RGBA8, GL_RGBA, mDesc.width, mDesc.height, i);
						break;
					case FramebufferTextureFormat::RED_INT:
						AttachColorTexture(mColorAttachments[i], mDesc.samples, GL_R32I, GL_RED_INTEGER, mDesc.width, mDesc.height, i);
						break;
				}
			}
		}

		if (mDepthAttachmentDesc.textureFormat != FramebufferTextureFormat::NONE)
		{
			CreateTextures(multisamples, &mDepthAttachment, 1);
			BindTexture(multisamples, mDepthAttachment);

			switch (mDepthAttachmentDesc.textureFormat)
			{
				case FramebufferTextureFormat::DEPTH24_STENCIL8:
					AttachDepthTexture(mDepthAttachment, mDesc.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, mDesc.width, mDesc.height);
					break;
			}
		}

		if (mColorAttachments.size() > 1)
		{
			RB_CORE_ASSERT(mColorAttachments.size() <= 4);
			GLenum buffers[4] = {
				GL_COLOR_ATTACHMENT0,
				GL_COLOR_ATTACHMENT1,
				GL_COLOR_ATTACHMENT2,
				GL_COLOR_ATTACHMENT3
			};

			glDrawBuffers((GLsizei)mColorAttachments.size(), buffers);
		}
		else if (mColorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}

		RB_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mId);
		glViewport(0, 0, mDesc.width, mDesc.height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	void OpenGLFramebuffer::Resize(uint32 width, uint32 height)
	{
		if (width == 0 || height == 0 || width > sMaxFramebufferSize || height > sMaxFramebufferSize)
		{
			RB_CORE_WARN("Attempted an invalid framebuffer resize to ({}, {})", width, height);
			return;
		}
		mDesc.width = width;
		mDesc.height = height;
		Invalidate();
	}

	int OpenGLFramebuffer::ReadPixel(uint32 attachmentIndex, int x, int y)
	{
		RB_PROFILE_FUNC();

		RB_CORE_ASSERT(attachmentIndex < mColorAttachments.size());

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

		return pixelData;
	}

	void OpenGLFramebuffer::ClearAttachment(uint32 attachmentIndex, int value)
	{
		RB_CORE_ASSERT(attachmentIndex < mColorAttachments.size());
		auto& desc = mColorAttachmentDesc[attachmentIndex];

		glClearTexImage(mColorAttachments[attachmentIndex], 0, GetGlFormat(desc.textureFormat), GL_INT, &value);
	}

}