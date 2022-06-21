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
// File Name: Application.cpp
// Date File Created: 06/13/2022 at 3:04 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"

#include "Application.h"


#include <glad/glad.h>

#include "platform/windows/Win64Window.h"


rebirth::Application* rebirth::Application::sInstance = nullptr;

static GLenum GetGlType(rebirth::ShaderDataType type)
{
	switch (type)
	{
	case rebirth::ShaderDataType::FLOAT:
	case rebirth::ShaderDataType::FLOAT2:
	case rebirth::ShaderDataType::FLOAT3:
	case rebirth::ShaderDataType::FLOAT4:
	case rebirth::ShaderDataType::MAT3:
	case rebirth::ShaderDataType::MAT4: return GL_FLOAT;
	case rebirth::ShaderDataType::INT:
	case rebirth::ShaderDataType::INT2:
	case rebirth::ShaderDataType::INT3:
	case rebirth::ShaderDataType::INT4: return GL_INT;
	case rebirth::ShaderDataType::BOOL: return GL_BOOL;
	case rebirth::ShaderDataType::NONE: break;
	}

	RB_CORE_ASSERT(false, "Unknown shader data type");
	return 0;
}

rebirth::Application::Application()
{
	RB_CORE_ASSERT(!sInstance, "Application already exists");
	sInstance = this;
	//mWindow = createScope<Win64Window>(Window::Create());
	mWindow = std::unique_ptr<Window>(Window::Create());
	mWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

	mImguiLayer = new ImguiLayer();
	PushOverlay(mImguiLayer);

	mVertexArray.reset(VertexArray::Create());

	float triVerts[3 * 7] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // lower left
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // lower right
		0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top center
	};

	SharedPtr<VertexBuffer> triVertBuffer;
	triVertBuffer.reset(VertexBuffer::Create(sizeof(triVerts), triVerts));

	BufferLayout triLayout = {
		{ ShaderDataType::FLOAT3, "aPos" },
		{ ShaderDataType::FLOAT4, "aColor" }
	};
	triVertBuffer->SetLayout(triLayout);
	mVertexArray->AddVertexBuffer(triVertBuffer);



	uint32_t triIndices[3] = { 0, 1, 2 };
	SharedPtr<IndexBuffer> triIndBuffer;
	triIndBuffer.reset(IndexBuffer::Create(sizeof(triIndices) / sizeof(uint32_t), triIndices));

	mVertexArray->SetIndexBuffer(triIndBuffer);


	float sq_verts[3 * 4] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	mSquareVtxArray.reset(VertexArray::Create());
	SharedPtr<VertexBuffer> svb;
	svb.reset(VertexBuffer::Create(sizeof(sq_verts), sq_verts));
	svb->SetLayout({ { ShaderDataType::FLOAT3, "aPos" } });
	mSquareVtxArray->AddVertexBuffer(svb);

	uint32_t sq_indices[6] = { 0, 1, 2, 2, 3, 0 };
	SharedPtr<IndexBuffer> sib;
	sib.reset(IndexBuffer::Create(sizeof(sq_indices) / sizeof(uint32_t), sq_indices));

	mSquareVtxArray->SetIndexBuffer(sib);

	const std::string vertSrc = R"(
	#version 330 core

	layout(location=0) in vec3 aPos;
	layout(location=1) in vec4 aColor;

	out vec3 vPos;
	out vec4 vColor;
	
	void main()
	{
		vPos = aPos + 0.5;
		vColor = aColor;
		gl_Position = vec4(aPos + 0.5, 1.0);
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

	out vec3 vPos;
	
	void main()
	{
		vPos = aPos + 0.5;
		gl_Position = vec4(aPos + 0.5, 1.0);
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

	mShader.reset(new Shader(vertSrc, fragSrc));
	mShaderNoColor.reset(new Shader(vertSrc2, fragSrc2));
}

void rebirth::Application::Run()
{
	while (mRunning)
	{
		glClearColor(0.05f, 0.05f, 0.05f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		mShaderNoColor->Bind();
		mSquareVtxArray->Bind();
		glDrawElements(GL_TRIANGLES, mSquareVtxArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);


		mShader->Bind();
		mVertexArray->Bind();
		glDrawElements(GL_TRIANGLES, mVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

		for (Layer* layer : mLayerStack)
		{
			if (layer)
				layer->OnUpdate();
		}

		mImguiLayer->Begin();
		for (Layer* layer : mLayerStack)
		{
			layer->OnImguiRender();
		}
		mImguiLayer->End();

		mWindow->OnUpdate();
	}
}

void rebirth::Application::OnEvent(Event& e)
{
	EventDispatcher disp(e);
	disp.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

	for (auto it = mLayerStack.end(); it != mLayerStack.begin();)
	{
		(*--it)->OnEvent(e);
		if (e.WasHandled())
		{
			break;
		}
	}
}

void rebirth::Application::PushLayer(Layer* layer)
{
	mLayerStack.PushLayer(layer);
}

void rebirth::Application::PushOverlay(Layer* overlay)
{
	mLayerStack.PushOverlay(overlay);
}

bool rebirth::Application::OnWindowClose(WindowCloseEvent& e)
{
	mRunning = false;
	return true;
}
