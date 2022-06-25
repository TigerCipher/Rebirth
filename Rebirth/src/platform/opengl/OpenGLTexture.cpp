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
// File Name: OpenGLTexture.cpp
// Date File Created: 6/22/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

#include <glad/glad.h>

namespace rebirth
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) :
		mPath(path)
	{
		RB_PROFILE_FUNC();
		RB_CORE_TRACE("Loading texture from file {}", path);
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		byte* data = nullptr;
		{
			RB_PROFILE_SCOPE("stbi_load in OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		// #NOTE perhaps default texture ? -Or perhaps better is to make default texture be when creating a quad/sprite/model/etc
		// If a texture fails to load, it should be an assert so we can catch it easier

		RB_CORE_ASSERT(data, "Failed to load texture file");
		mWidth = width;
		mHeight = height;

		if (channels == 4)
		{
			mInternalFormat = GL_RGBA8;
			mDataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			mInternalFormat = GL_RGB8;
			mDataFormat = GL_RGB;
		}

		RB_CORE_ASSERT(mInternalFormat & mDataFormat, "Image format not supported");

		glCreateTextures(GL_TEXTURE_2D, 1, &mId);
		glTextureStorage2D(mId, 1, mInternalFormat, mWidth, mHeight);

		glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(mId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(mId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(mId, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

		RB_CORE_TRACE("Texture {} loaded", path);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint width, uint height) :
		mWidth(width), mHeight(height)
	{
		RB_PROFILE_FUNC();
		mInternalFormat = GL_RGBA8;
		mDataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &mId);
		glTextureStorage2D(mId, 1, mInternalFormat, mWidth, mHeight);

		glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(mId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(mId, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		RB_PROFILE_FUNC();
		glDeleteTextures(1, &mId);
	}

	void OpenGLTexture2D::Bind(const uint slot /*= 0*/) const
	{
		RB_PROFILE_FUNC();
		glBindTextureUnit(slot, mId);
	}

	void OpenGLTexture2D::SetData(void* data, uint size)
	{
		RB_PROFILE_FUNC();
		RB_CORE_ASSERT(size == mWidth * mHeight * (mDataFormat == GL_RGBA ? 4 : 3), "Data must contain entire texture");
		glTextureSubImage2D(mId, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, data);
	}

}

