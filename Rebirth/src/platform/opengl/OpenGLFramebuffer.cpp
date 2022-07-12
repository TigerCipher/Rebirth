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

	static void AttachColorTexture(uint32 id, int samples, GLenum format, uint32 width, uint32 height, int index)
	{
		bool multisamples = samples > 1;
		if (multisamples)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

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

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferDesc& spec) :
		mSpecification(spec)
	{
		for (auto format : mSpecification.attachements.attachments)
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
		glDeleteTextures(mColorAttachments.size(), mColorAttachments.data());
		glDeleteTextures(1, &mDepthAttachment);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		RB_PROFILE_FUNC();

		if(mId)
		{
			glDeleteFramebuffers(1, &mId);
			glDeleteTextures(mColorAttachments.size(), mColorAttachments.data());
			glDeleteTextures(1, &mDepthAttachment);
			mColorAttachments.clear();
			mDepthAttachment = 0;
		}

		glCreateFramebuffers(1, &mId);
		glBindFramebuffer(GL_FRAMEBUFFER, mId);


		// Attachments

		bool multisamples = mSpecification.samples > 1;
		if (mColorAttachmentDesc.size())
		{
			mColorAttachments.resize(mColorAttachmentDesc.size());
			CreateTextures(multisamples, mColorAttachments.data(), mColorAttachments.size());
			for (size_t i = 0; i < mColorAttachments.size(); i++)
			{
				BindTexture(multisamples, mColorAttachments[i]);
				switch (mColorAttachmentDesc[i].textureFormat)
				{
					case FramebufferTextureFormat::RGBA8:
						AttachColorTexture(mColorAttachments[i], mSpecification.samples, GL_RGBA8, mSpecification.width, mSpecification.height, i);
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
					AttachDepthTexture(mDepthAttachment, mSpecification.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, mSpecification.width, mSpecification.height);
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

			glDrawBuffers(mColorAttachments.size(), buffers);
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
		glViewport(0, 0, mSpecification.width, mSpecification.height);
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
		mSpecification.width = width;
		mSpecification.height = height;
		Invalidate();
	}

}