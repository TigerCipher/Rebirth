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
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include "rebirth/core/Application.h"

#ifndef GLSL_MAX_SHADERS_PER_FILE
#	define GLSL_MAX_SHADERS_PER_FILE 2
#endif

namespace rebirth
{

	static GLenum GetShaderType(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
		RB_CORE_ASSERT(false, "Unknown shader type");
		return 0;
	}

	static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
	{
		switch (stage)
		{
			case GL_VERTEX_SHADER: return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
		}

		RB_CORE_ASSERT(false);
		return (shaderc_shader_kind)0;
	}

	static const char* GLShaderStageToString(GLenum stage)
	{
		switch (stage)
		{
			case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
			case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
		}

		RB_CORE_ASSERT(false);
		return nullptr;
	}

	static const char* GetCacheDirectory()
	{
		return "assets/cache/shader/opengl";
	}

	static const char* GLShaderStageCachedOpenGLFileExtension(GLenum stage)
	{
		switch (stage)
		{
			case GL_VERTEX_SHADER: return ".cached_opengl.vert";
			case GL_FRAGMENT_SHADER: return ".cached_opengl.frag";
		}

		RB_CORE_ASSERT(false);
		return nullptr;
	}

	static const char* GLShaderStageCachedVulkanFileExtension(GLenum stage)
	{
		switch (stage)
		{
			case GL_VERTEX_SHADER: return ".cached_vulkan.vert";
			case GL_FRAGMENT_SHADER: return ".cached_vulkan.frag";
		}

		RB_CORE_ASSERT(false);
		return nullptr;
	}

	static void CreateCacheDirectory()
	{
		static int count = 0;
		std::string cacheDir = GetCacheDirectory();
		if (!std::filesystem::exists(cacheDir))
			std::filesystem::create_directories(cacheDir);
		else if (count == 0)
		{
			auto args = Application::Instance().GetCommandLineArgs();
			if (args.count > 1)
			{
				for (int i = 1; i < args.count; i++)
				{
					std::string arg = args[i];
					if (arg == "-f" || arg == "-F" || arg == "--force-shader-compile")
					{
						RB_CORE_TRACE("force-shader-compile command line argument detected, deleting cached shaders and recompiling");
						std::filesystem::remove_all(cacheDir);
						std::filesystem::create_directories(cacheDir);
						break;
					}
				}
			}
			count++;
		}
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) :
		mFilepath(filepath)
	{
		RB_PROFILE_FUNC();

		CreateCacheDirectory();

		std::string src = Read(filepath);
		auto sources = Preprocess(src);

		CompileOrGetVulkanBinaries(sources);
		CompileOrGetOpenGLBinaries();
		CreateProgram();

		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		mName = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) :
		mName(name)
	{
		RB_PROFILE_FUNC();
		std::unordered_map<uint32, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;


		CompileOrGetVulkanBinaries(sources);
		CompileOrGetOpenGLBinaries();
		CreateProgram();
	}



	OpenGLShader::OpenGLShader(const std::string& vertexPath, const std::string& pixelPath)
	{
		RB_PROFILE_FUNC();

		CreateCacheDirectory();

		std::string vertSrc = Read(vertexPath);
		std::string fragSrc = Read(pixelPath);
		std::unordered_map<uint32, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertSrc;
		sources[GL_FRAGMENT_SHADER] = fragSrc;
		auto lastSlash = vertexPath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = vertexPath.rfind('.');
		auto count = lastDot == std::string::npos ? vertexPath.size() - lastSlash : lastDot - lastSlash;
		mName = vertexPath.substr(lastSlash, count);

		mFilepath = mName;

		CompileOrGetVulkanBinaries(sources);
		CompileOrGetOpenGLBinaries();
		CreateProgram();

	}

	OpenGLShader::~OpenGLShader()
	{
		RB_PROFILE_FUNC();
		glDeleteShader(mId);
	}

	void OpenGLShader::Bind() const
	{
		RB_PROFILE_FUNC();
		glUseProgram(mId);
	}

	void OpenGLShader::Unbind() const
	{
		RB_PROFILE_FUNC();
		glUseProgram(0);
	}


	std::string OpenGLShader::Read(const std::string& filepath)
	{
		RB_PROFILE_FUNC();
		RB_CORE_INFO("Loading shader file [{}]", filepath);
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

		RB_CORE_INFO("Shader file read");
		return result;
	}

	std::unordered_map<uint32, std::string> OpenGLShader::Preprocess(const std::string& src)
	{
		RB_PROFILE_FUNC();
		RB_CORE_INFO("Preprocessing shader file");
		std::unordered_map<uint32, std::string> sources;

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
			RB_CORE_ASSERT(nextLinePos != std::string::npos, "Shader syntax error");
			pos = src.find(typeDirective, nextLinePos);
			sources[GetShaderType(type)] = src.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? src.size() - 1 : nextLinePos));
		}

		RB_CORE_INFO("Finished preprocessing shader file");
		return sources;
	}

	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<uint32, std::string>& sources)
	{
		RB_PROFILE_FUNC();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDir = GetCacheDirectory();
		auto& shaderData = mVulkanSPIRV;
		shaderData.clear();

		for (auto&& [stage, source] : sources)
		{
			std::filesystem::path shaderFilepath = mFilepath;
			std::filesystem::path cachePath = cacheDir / (shaderFilepath.filename().string() + GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachePath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32));
				in.read((char*)data.data(), size);
			}
			else
			{
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, GLShaderStageToShaderC(stage), mFilepath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					RB_CORE_ERROR(module.GetErrorMessage());
					RB_CORE_ASSERT(false, "Failed to compile shader during vulkan stage [{}]", GLShaderStageToString(stage));
				}

				shaderData[stage] = std::vector<uint32>(module.cbegin(), module.cend());

				std::ofstream out(cachePath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data] : shaderData)
		{
			Reflect(stage, data);
		}
	}

	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		RB_PROFILE_FUNC();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDir = GetCacheDirectory();
		auto& shaderData = mOpenGLSPIRV;
		shaderData.clear();
		mOpenGLsrc.clear();

		for (auto&& [stage, spirv] : mVulkanSPIRV)
		{
			std::filesystem::path shaderFilepath = mFilepath;
			std::filesystem::path cachePath = cacheDir / (shaderFilepath.filename().string() + GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachePath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32));
				in.read((char*)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				mOpenGLsrc[stage] = glslCompiler.compile();
				auto& source = mOpenGLsrc[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, GLShaderStageToShaderC(stage), mFilepath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					RB_CORE_ERROR(module.GetErrorMessage());
					RB_CORE_ASSERT(false, "Failed to compile shader during opengl stage [{}]", GLShaderStageToString(stage));
				}

				shaderData[stage] = std::vector<uint32>(module.cbegin(), module.cend());

				std::ofstream out(cachePath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32));
					out.flush();
					out.close();
				}
			}
		}
	}

	void OpenGLShader::CreateProgram()
	{
		RB_CORE_INFO("Creating glsl shader program");
		uint32 prog = glCreateProgram();
		std::vector<GLuint> shaderIds;
		for (auto&& [stage, spirv] : mOpenGLSPIRV)
		{
			RB_CORE_INFO("Creating and attaching {} shader", GLShaderStageToString(stage));
			GLuint id = shaderIds.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &id, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32));
			glSpecializeShader(id, "main", 0, nullptr, nullptr);
			glAttachShader(prog, id);
		}

		RB_CORE_INFO("Linking shader program {}", prog);
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

		mId = prog;
		for (auto id : shaderIds)
		{
			glDetachShader(prog, id);
			glDeleteShader(id);
		}


		RB_CORE_INFO("Shader program {} successfully compiled", mId);
	}

	void OpenGLShader::Reflect(uint32 stage, const std::vector<uint32>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		std::string logMsg = R"(OpenGL::Reflect - {} {}
	{} uniform buffers
	{} resources
			)";

		RB_CORE_TRACE(logMsg, GLShaderStageToString(stage), mFilepath, resources.uniform_buffers.size(), resources.sampled_images.size());

		std::stringstream ss;
		ss << "Uniform Buffers:\n";
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32 bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32 binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			ss << "\t" << resource.name;
			ss << "\n\t\tSize = " << bufferSize;
			ss << "\n\t\tBinding = " << binding;
			ss << "\n\t\tMembers = " << memberCount;
		}
		RB_CORE_TRACE(ss.str());
	}


	void OpenGLShader::SetInt(const std::string& name, const int value)
	{
		RB_PROFILE_FUNC();
		SetUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, const int* values, const uint32 count)
	{
		RB_PROFILE_FUNC();
		SetUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, const float value)
	{
		RB_PROFILE_FUNC();
		SetUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& vec)
	{
		RB_PROFILE_FUNC();
		SetUniformVec2(name, vec);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& vec)
	{
		RB_PROFILE_FUNC();
		SetUniformVec3(name, vec);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& vec)
	{
		RB_PROFILE_FUNC();
		SetUniformVec4(name, vec);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix)
	{
		RB_PROFILE_FUNC();
		SetUniformMat3(name, matrix);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix)
	{
		RB_PROFILE_FUNC();
		SetUniformMat4(name, matrix);
	}

	void OpenGLShader::SetUniformInt(const std::string& name, const int value)
	{
		int loc = glGetUniformLocation(mId, name.c_str());
		glUniform1i(loc, value);
	}

	void OpenGLShader::SetUniformIntArray(const std::string& name, const int* values, const uint32 count)
	{
		int loc = glGetUniformLocation(mId, name.c_str());
		glUniform1iv(loc, count, values);
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
		uint32 loc = glGetUniformLocation(mId, name.c_str());
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));

	}
}

