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
// File Name: Shader.cpp
// Date File Created: 06/19/2022 at 2:50 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"

#include "Shader.h"

#include "Renderer.h"
#include "platform/opengl/OpenGLShader.h"

namespace rebirth
{

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::NONE:
				RB_CORE_ASSERT(false, "Must use a graphics API");
				return nullptr;

			case RendererAPI::API::OPENGL: return createRef<OpenGLShader>(name, vertexSrc, fragSrc);
		}

		RB_CORE_ASSERT(false, "Unknown graphics API");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::NONE:
				RB_CORE_ASSERT(false, "Must use a graphics API");
				return nullptr;

			case RendererAPI::API::OPENGL: return createRef<OpenGLShader>(filepath);
		}

		RB_CORE_ASSERT(false, "Unknown graphics API");
		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		RB_CORE_ASSERT(mShaders.find(name) == mShaders.end(), "Shader already exists in the library");
		mShaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		RB_CORE_ASSERT(mShaders.find(name) != mShaders.end(), "Shader was not found in the library");
		return mShaders[name];
	}

	Ref<rebirth::Shader> ShaderLibrary::operator[](const std::string& name)
	{
		return Get(name);
	}

}