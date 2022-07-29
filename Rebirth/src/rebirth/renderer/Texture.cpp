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
// File Name: Texture.cpp
// Date File Created: 6/22/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "platform/opengl/OpenGLTexture.h"

namespace rebirth
{
	// Temporary texture asset management system
	static std::unordered_map<std::string, Ref<Texture2D>> sTextures;

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		if (sTextures.find(path) != sTextures.end())
		{
			return sTextures[path];
		}

		switch (Renderer::GetAPI())
		{
			case GraphicsAPI::API::NONE:
			{
				RB_CORE_ASSERT(false, "Must use a graphics API");
				return nullptr;
			}

			case GraphicsAPI::API::OPENGL:
			{
				Ref<Texture2D> tex = createRef<OpenGLTexture2D>(path);
				sTextures[path] = tex;
				return tex;
			}
		}

		RB_CORE_ASSERT(false, "Unknown graphics API");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(uint32 width, uint32 height)
	{
		switch (Renderer::GetAPI())
		{
			case GraphicsAPI::API::NONE:
			{
				RB_CORE_ASSERT(false, "Must use a graphics API");
				return nullptr;
			}

			case GraphicsAPI::API::OPENGL: return createRef<OpenGLTexture2D>(width, height);
		}

		RB_CORE_ASSERT(false, "Unknown graphics API");
		return nullptr;
	}

	Reference<Texture2D> Texture2D::Create(const std::string& path, int temp)
	{
		switch (Renderer::GetAPI())
		{
			case GraphicsAPI::API::NONE:
			{
				RB_CORE_ASSERT(false, "Must use a graphics API");
				return nullptr;
			}

			case GraphicsAPI::API::OPENGL:
			{
				return createReference<OpenGLTexture2D>(path);
			}
		}

		RB_CORE_ASSERT(false, "Unknown graphics API");
		return nullptr;
	}
}
