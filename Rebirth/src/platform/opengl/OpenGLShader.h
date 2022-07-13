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


namespace rebirth
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertexPath, const std::string& pixelPath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		const std::string& GetName() const override { return mName; }

		void SetInt(const std::string& name, const int value) override;
		void SetIntArray(const std::string& name, const int* values, const uint32 count);
		void SetFloat(const std::string& name, const float value) override;
		void SetFloat2(const std::string& name, const glm::vec2& vec) override;
		void SetFloat3(const std::string& name, const glm::vec3& vec) override;
		void SetFloat4(const std::string& name, const glm::vec4& vec) override;
		void SetMat3(const std::string& name, const glm::mat3& matrix) override;
		void SetMat4(const std::string& name, const glm::mat4& matrix) override;

		void SetUniformInt(const std::string& name, const int value);
		void SetUniformIntArray(const std::string& name, const int* values, const uint32 count);
		void SetUniformFloat(const std::string& name, const float value);
		void SetUniformVec2(const std::string& name, const glm::vec2& vec);
		void SetUniformVec3(const std::string& name, const glm::vec3& vec);
		void SetUniformVec4(const std::string& name, const glm::vec4& vec);
		void SetUniformMat3(const std::string& name, const glm::mat3& matrix);
		void SetUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		std::string Read(const std::string& filepath);
		std::unordered_map<uint32, std::string> Preprocess(const std::string& src);
		void CompileOrGetVulkanBinaries(const std::unordered_map<uint32, std::string>& sources);
		void CompileOrGetOpenGLBinaries();

		void CreateProgram();
		void Reflect(uint32 stage, const std::vector<uint32>& shaderData);


		uint32 mId = 0;
		std::string mName;
		std::string mFilepath;

		std::unordered_map<uint32, std::vector<uint32>> mVulkanSPIRV;
		std::unordered_map<uint32, std::vector<uint32>> mOpenGLSPIRV;
		std::unordered_map<uint32, std::string> mOpenGLsrc;
	};
}

