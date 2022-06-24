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
// File Name: OpenGLShader.cpp
// Date File Created: 6/22/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "OpenGLShader.h"

#include <fstream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#ifndef GLSL_MAX_SHADERS_PER_FILE
#	define GLSL_MAX_SHADERS_PER_FILE 2
#endif

namespace rebirth
{

	static uint GetShaderType(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
		RB_CORE_ASSERT(false, "Unknown shader type");
		return 0;
	}

	// TODO Support using multiple files rather than only a single file
	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string src = Read(filepath);
		auto sources = Preprocess(src);
		Compile(sources);

		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		mName = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) :
		mName(name)
	{
		std::unordered_map<uint, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}



	OpenGLShader::OpenGLShader(const std::string& vertexPath, const std::string& pixelPath)
	{
		std::string vertSrc = Read(vertexPath);
		std::string fragSrc = Read(pixelPath);
		std::unordered_map<uint, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertSrc;
		sources[GL_FRAGMENT_SHADER] = fragSrc;
		Compile(sources);

		auto lastSlash = vertexPath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = vertexPath.rfind('.');
		auto count = lastDot == std::string::npos ? vertexPath.size() - lastSlash : lastDot - lastSlash;
		mName = vertexPath.substr(lastSlash, count);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteShader(mId);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(mId);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetUniformInt(const std::string& name, const int value)
	{
		int loc = glGetUniformLocation(mId, name.c_str());
		glUniform1i(loc, value);
	}

	void OpenGLShader::SetUniformFloat(const std::string& name, const float value)
	{
		int loc = glGetUniformLocation(mId, name.c_str());
		glUniform1f(loc, value);
	}

	void OpenGLShader::SetUniformVec2(const std::string& name, const glm::vec2& vec)
	{
		int loc = glGetUniformLocation(mId, name.c_str());
		glUniform2f(loc, vec.x, vec.y);
	}

	void OpenGLShader::SetUniformVec3(const std::string& name, const glm::vec3& vec)
	{
		int loc = glGetUniformLocation(mId, name.c_str());
		glUniform3f(loc, vec.x, vec.y, vec.z);
	}

	void OpenGLShader::SetUniformVec4(const std::string& name, const glm::vec4& vec)
	{
		int loc = glGetUniformLocation(mId, name.c_str());
		glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
	}

	void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		int loc = glGetUniformLocation(mId, name.c_str());
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		int loc = glGetUniformLocation(mId, name.c_str());
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
	}


	std::string OpenGLShader::Read(const std::string& filepath)
	{
		RB_CORE_TRACE("Loading shader file [{}]", filepath);
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			RB_CORE_ERROR("Failed to open shader file [{}]", filepath);
		}

		RB_CORE_TRACE("Shader file read");
		return result;
	}

	std::unordered_map<uint, std::string> OpenGLShader::Preprocess(const std::string& src)
	{
		RB_CORE_TRACE("Preprocessing shader file");
		std::unordered_map<uint, std::string> sources;

		const char* typeDirective = "#type";
		size_t typeDirectiveLength = strlen(typeDirective);
		size_t pos = src.find(typeDirective, 0);

		while (pos != std::string::npos)
		{
			size_t eol = src.find_first_of("\r\n", pos);
			RB_CORE_ASSERT(eol != std::string::npos, "Shader syntax error");
			size_t beg = pos + typeDirectiveLength + 1;
			std::string type = src.substr(beg, eol - beg);
			RB_CORE_ASSERT(GetShaderType(type), "Invalid shader type preprocessor directive");

			size_t nextLinePos = src.find_first_not_of("\r\n", eol);
			pos = src.find(typeDirective, nextLinePos);
			sources[GetShaderType(type)] = src.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? src.size() - 1 : nextLinePos));
		}

		RB_CORE_TRACE("Finished preprocessing shader file");
		return sources;
	}

	void OpenGLShader::Compile(const std::unordered_map<uint, std::string>& sources)
	{
		uint prog = glCreateProgram();
		RB_CORE_ASSERT(sources.size() <= GLSL_MAX_SHADERS_PER_FILE, "Rebirth currently only supports two shaders in a single file");
		RB_CORE_TRACE("Compiling shader program {}", prog);
		std::array<uint, GLSL_MAX_SHADERS_PER_FILE> shaderIds;
		int shaderIdIndex = 0;
		for (auto& kv : sources)
		{
			uint type = kv.first;
			const std::string& src = kv.second;
			RB_CORE_TRACE("Compiling shader type {}", type == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER"
													: type == GL_FRAGMENT_SHADER ? "GL_FRAGMENT_SHADER"
													: "UNKNOWN");

			uint shader = glCreateShader(type);
			const char* source = src.c_str();
			glShaderSource(shader, 1, &source, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				RB_CORE_ERROR("{}", infoLog.data());
				RB_CORE_ASSERT(false, "Failed to compile shader");
				return;
			}

			glAttachShader(prog, shader);
			shaderIds[shaderIdIndex++] = shader;
			RB_CORE_TRACE("Shader {} compiled and attached to shader program {}", shader, prog);
		}

		RB_CORE_TRACE("Linking shader program {}", prog);
		glLinkProgram(prog);

		GLint isLinked = 0;
		glGetProgramiv(prog, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(prog, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(prog);

			for (auto id : shaderIds)
			{
				glDeleteShader(id);
			}

			RB_CORE_ERROR("{}", infoLog.data());
			RB_CORE_ASSERT(false, "Failed to link shader");
			return;
		}

		for (auto id : shaderIds)
		{
			glDetachShader(prog, id);
		}

		mId = prog;

		RB_CORE_TRACE("Shader program {} successfully compiled", mId);
	}

}

