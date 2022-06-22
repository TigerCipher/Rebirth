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

class SampleLayer final : public rebirth::Layer
{
public:
	SampleLayer() : Layer("Sample"), mCamera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		mVertexArray.reset(rebirth::VertexArray::Create());

		float triVerts[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // lower left
			0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // lower right
			0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top center
		};

		SharedPtr<rebirth::VertexBuffer> triVertBuffer;
		triVertBuffer.reset(rebirth::VertexBuffer::Create(sizeof(triVerts), triVerts));

		rebirth::BufferLayout triLayout = {
			{ rebirth::ShaderDataType::FLOAT3, "aPos" },
			{ rebirth::ShaderDataType::FLOAT4, "aColor" }
		};
		triVertBuffer->SetLayout(triLayout);
		mVertexArray->AddVertexBuffer(triVertBuffer);



		uint32_t triIndices[3] = { 0, 1, 2 };
		SharedPtr<rebirth::IndexBuffer> triIndBuffer;
		triIndBuffer.reset(rebirth::IndexBuffer::Create(sizeof(triIndices) / sizeof(uint32_t), triIndices));

		mVertexArray->SetIndexBuffer(triIndBuffer);


		float sq_verts[3 * 4] =
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};

		mSquareVtxArray.reset(rebirth::VertexArray::Create());
		SharedPtr<rebirth::VertexBuffer> svb;
		svb.reset(rebirth::VertexBuffer::Create(sizeof(sq_verts), sq_verts));
		svb->SetLayout({ { rebirth::ShaderDataType::FLOAT3, "aPos" } });
		mSquareVtxArray->AddVertexBuffer(svb);

		uint32_t sq_indices[6] = { 0, 1, 2, 2, 3, 0 };
		SharedPtr<rebirth::IndexBuffer> sib;
		sib.reset(rebirth::IndexBuffer::Create(sizeof(sq_indices) / sizeof(uint32_t), sq_indices));

		mSquareVtxArray->SetIndexBuffer(sib);

		const std::string vertSrc = R"(
	#version 330 core

	layout(location=0) in vec3 aPos;
	layout(location=1) in vec4 aColor;

	uniform mat4 uViewProj;

	out vec3 vPos;
	out vec4 vColor;

	
	void main()
	{
		vPos = aPos;
		vColor = aColor;
		gl_Position = uViewProj * vec4(aPos, 1.0);
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

		const std::string vertSrc2 = R"(
	#version 330 core

	layout(location=0) in vec3 aPos;

	uniform mat4 uViewProj;

	out vec3 vPos;

	
	void main()
	{
		vPos = aPos;
		gl_Position = uViewProj * vec4(aPos, 1.0);
	}

	)";

		const std::string fragSrc2 = R"(
	#version 330 core

	layout(location=0) out vec4 color;

	in vec3 vPos;

	void main()
	{
		color = vec4(0.2, 0.4, 0.7, 1.0);
	}

	)";

		mShader.reset(new rebirth::Shader(vertSrc, fragSrc));
		mShaderNoColor.reset(new rebirth::Shader(vertSrc2, fragSrc2));
	}

	void OnUpdate() override
	{
		//mCamera.SetPosition({ 0.5f, 0.5f, 0.0f });
		mCamera.SetRotation(45.0f);
		rebirth::Renderer::BeginScene(mCamera);
		rebirth::Renderer::Submit(mShaderNoColor, mSquareVtxArray);
		rebirth::Renderer::Submit(mShader, mVertexArray);
		rebirth::Renderer::EndScene();
	}

	void OnEvent(rebirth::Event& e) override
	{
	}

	void OnImguiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Testing Rebirth Engine UI");
		ImGui::End();
	}

private:
	SharedPtr<rebirth::Shader> mShader;
	SharedPtr<rebirth::Shader> mShaderNoColor;
	SharedPtr<rebirth::VertexArray> mVertexArray;

	SharedPtr<rebirth::VertexArray> mSquareVtxArray;

	rebirth::OrthoCamera mCamera;
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
