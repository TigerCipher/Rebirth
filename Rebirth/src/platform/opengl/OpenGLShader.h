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
// File Name: OpenGLShader.h
// Date File Created: 6/22/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "rebirth/renderer/Shader.h"
#include <glm/glm.hpp>

namespace rebirth
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		void SetUniformInt(const std::string& name, const int value);
		void SetUniformFloat(const std::string& name, const float value);
		void SetUniformVec2(const std::string & name, const glm::vec2 & vec);
		void SetUniformVec3(const std::string& name, const glm::vec3& vec);
		void SetUniformVec4(const std::string& name, const glm::vec4& vec);
		void SetUniformMat3(const std::string& name, const glm::mat3& matrix);
		void SetUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		uint32_t mId = 0;
		std::string Read(const std::string& filepath);
		std::unordered_map<uint, std::string> Preprocess(const std::string& src);
		void Compile(const std::unordered_map<uint, std::string>& sources);
	};
}

