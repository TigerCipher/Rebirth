// ------------------------------------------------------------------------------
// 
// Sandbox
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
// File Name: App.cpp
// Date File Created: 06/13/2022 at 2:16 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include <Rebirth.h>

#include <imgui/imgui.h>


#include <glm/gtc/matrix_transform.hpp>
#include "platform/opengl/OpenGLShader.h"
#include <glm/gtc/type_ptr.hpp>

class SampleLayer final : public rebirth::Layer
{
public:
	SampleLayer() : Layer("Sample"), mCameraController(1920.0f / 1080.0f, true)
	{
		mVertexArray.reset(rebirth::VertexArray::Create());

		float triVerts[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // lower left
			0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // lower right
			0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top center
		};

		Ref<rebirth::VertexBuffer> triVertBuffer;
		triVertBuffer.reset(rebirth::VertexBuffer::Create(sizeof(triVerts), triVerts));

		rebirth::BufferLayout triLayout = {
			{ rebirth::ShaderDataType::FLOAT3, "aPos" },
			{ rebirth::ShaderDataType::FLOAT4, "aColor" }
		};
		triVertBuffer->SetLayout(triLayout);
		mVertexArray->AddVertexBuffer(triVertBuffer);



		uint32_t triIndices[3] = { 0, 1, 2 };
		Ref<rebirth::IndexBuffer> triIndBuffer;
		triIndBuffer.reset(rebirth::IndexBuffer::Create(sizeof(triIndices) / sizeof(uint32_t), triIndices));

		mVertexArray->SetIndexBuffer(triIndBuffer);


		float sq_verts[5 * 4] =
		{
			/* Positions              Tex coords*/
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
			0.5f, 0.5f, 0.0f,		1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		0.0f, 1.0f
		};

		mSquareVtxArray.reset(rebirth::VertexArray::Create());
		Ref<rebirth::VertexBuffer> svb;
		svb.reset(rebirth::VertexBuffer::Create(sizeof(sq_verts), sq_verts));
		svb->SetLayout({
				{ rebirth::ShaderDataType::FLOAT3, "aPos" },
				{ rebirth::ShaderDataType::FLOAT2, "aTexCoord" }
			});
		mSquareVtxArray->AddVertexBuffer(svb);

		uint32_t sq_indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<rebirth::IndexBuffer> sib;
		sib.reset(rebirth::IndexBuffer::Create(sizeof(sq_indices) / sizeof(uint32_t), sq_indices));

		mSquareVtxArray->SetIndexBuffer(sib);

		const std::string vertSrc = R"(
	#version 330 core

	layout(location=0) in vec3 aPos;
	layout(location=1) in vec4 aColor;

	uniform mat4 uViewProj;
	uniform mat4 uModelTransform;

	out vec3 vPos;
	out vec4 vColor;

	
	void main()
	{
		vPos = aPos;
		vColor = aColor;
		gl_Position = uViewProj * uModelTransform * vec4(aPos, 1.0);
	}

	)";

		const std::string fragSrc = R"(
	#version 330 core

	layout(location=0) out vec4 color;

	in vec3 vPos;
	in vec4 vColor;

	void main()
	{
		color = vec4(vPos * 0.5 + 0.5, 1.0);
		color = vColor;
	}

	)";


		mShader = rebirth::Shader::Create("Triangle", vertSrc, fragSrc);

		mShaderColor = rebirth::Shader::Create("assets/shaders/FlatColor.glsl");

		auto texShader = mLibrary.Load("assets/shaders/Texture.vert", "assets/shaders/Texture.frag");

		mTexture = rebirth::Texture2D::Create("assets/textures/default.png");
		mTextureGun = rebirth::Texture2D::Create("assets/textures/Gun1.png");

		std::dynamic_pointer_cast<rebirth::OpenGLShader>(texShader)->Bind();
		std::dynamic_pointer_cast<rebirth::OpenGLShader>(texShader)->SetUniformInt("uTexture", 0);
	}

	void OnUpdate(rebirth::Timestep timestep) override
	{
		// tick/update
		mCameraController.OnUpdate(timestep);

		// render

		rebirth::RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1.0f });
		rebirth::RenderCommand::Clear();

		rebirth::Renderer::BeginScene(mCameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f));



		mShaderColor->Bind();
		std::dynamic_pointer_cast<rebirth::OpenGLShader>(mShaderColor)->SetUniformVec4("uColor", mSquareColor);

		for (int i = 0; i < 25; i++)
		{
			for (int j = 0; j < 25; j++)
			{
				glm::vec3 pos(j * 0.06f, i * 0.06f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				rebirth::Renderer::Submit(mShaderColor, mSquareVtxArray, transform);
			}
		}

		mTexture->Bind();
		auto texShader = mLibrary["Texture"];
		rebirth::Renderer::Submit(texShader, mSquareVtxArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		mTextureGun->Bind();
		rebirth::Renderer::Submit(texShader, mSquareVtxArray,
			glm::translate(glm::mat4(1.0f), glm::vec3(-0.25f, -0.25f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		//rebirth::Renderer::Submit(mShader, mVertexArray);
		rebirth::Renderer::EndScene();
	}

	void OnEvent(rebirth::Event& e) override
	{
		mCameraController.OnEvent(e);
		//rebirth::EventDispatcher disp(e);
		//disp.Dispatch<rebirth::KeyPressedEvent>(BIND_EVENT_FUNC(SampleLayer::OnKeyPressed));
	}


	void OnImguiRender() override
	{
		ImGui::Begin("Color Changer");
		ImGui::ColorPicker3("Square Color", glm::value_ptr(mSquareColor));
		ImGui::End();
	}

private:

	rebirth::ShaderLibrary mLibrary;

	Ref<rebirth::Shader> mShader;
	Ref<rebirth::Shader> mShaderColor;
	Ref<rebirth::VertexArray> mVertexArray;
	Ref<rebirth::VertexArray> mSquareVtxArray;
	Ref<rebirth::Texture> mTexture;
	Ref<rebirth::Texture> mTextureGun;

	rebirth::OrthoCameraController mCameraController;

	glm::vec4 mSquareColor{ 0.6f, 0.2f, 0.2f, 1.0f };
	
};

class Sandbox final : public rebirth::Application
{
public:
	Sandbox()
	{
		PushLayer(new SampleLayer());
		//PushOverlay(new rebirth::ImguiLayer());
	}
	~Sandbox() override = default;

};


rebirth::Application* rebirth::CreateApplication()
{
	return new Sandbox();
}
