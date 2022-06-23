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
		RB_CORE_TRACE("Loading texture from file {}", path);
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		byte* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		//TODO Perhaps default texture instead?
		RB_CORE_ASSERT(data, "Failed to load texture file");
		mWidth = width;
		mHeight = height;

		uint internalFormat = 0;
		uint dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		RB_CORE_ASSERT(internalFormat & dataFormat, "Image format not supported");

		glCreateTextures(GL_TEXTURE_2D, 1, &mId);
		glTextureStorage2D(mId, 1, internalFormat, mWidth, mHeight);
		glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureSubImage2D(mId, 0, 0, 0, mWidth, mHeight, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);

		RB_CORE_TRACE("Texture {} loaded", path);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &mId);
	}

	void OpenGLTexture2D::Bind(const uint slot /*= 0*/) const
	{
		glBindTextureUnit(slot, mId);
	}

}

