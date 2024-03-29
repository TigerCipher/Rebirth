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
// File Name: OpenGLTexture.h
// Date File Created: 6/22/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once
#include "rbpch.h"

#include "rebirth/renderer/Texture.h"

namespace rebirth
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(uint32 width, uint32 height);
		virtual ~OpenGLTexture2D();

		void Bind(uint32 slot = 0) const override;

		void SetData(void* data, uint32 size) override;

		int GetWidth() const override
		{
			return mWidth;
		}

		int GetHeight() const override
		{
			return mHeight;
		}

		uint32 GetId() const override { return mId; }

		const std::string& GetPath() const override { return mPath; }

		bool IsLoaded() const override { return mLoaded; }

		bool operator==(const Texture& other) const override
		{
			return mId == other.GetId();
		}
	private:
		int mWidth;
		int mHeight;
		uint32 mId;
		uint32 mInternalFormat;
		uint32 mDataFormat;
		bool mLoaded = false;

		std::string mPath;
	};
}

