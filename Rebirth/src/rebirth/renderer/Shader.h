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
// File Name: Shader.h
// Date File Created: 06/19/2022 at 2:50 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once

namespace rebirth
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		virtual void SetInt(const std::string& name, const int value) = 0;
		virtual void SetIntArray(const std::string& name, const int* values, const uint32 count) = 0;
		virtual void SetFloat(const std::string& name, const float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& vec) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& vec) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& vec) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& matrix) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) = 0;

		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc);
		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& vertexPath, const std::string& pixelPath);
	};

	class ShaderLibrary
	{
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& vertexPath, const std::string& pixelPath);
		Ref<Shader> Load(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc);

		Ref<Shader> Get(const std::string& name);
		Ref<Shader> operator[](const std::string& name);
	private:
		std::unordered_map<std::string, Ref<Shader>> mShaders;
	};
}
