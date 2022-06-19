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

rebirth::Application::Application()
{
	RB_CORE_ASSERT(!sInstance, "Application already exists");
	sInstance = this;
	//mWindow = createScope<Win64Window>(Window::Create());
	mWindow = std::unique_ptr<Window>(Window::Create());
	mWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

	mImguiLayer = new ImguiLayer();
	PushOverlay(mImguiLayer);

	// TEMPORARY
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);

	float verts[3*3] =
	{
		-0.5f, -0.5f, 0.0f, // lower left
		0.5f, -0.5f, 0.0f, // lower right
		0.0f, 0.5f, 0.0f, // top center
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

	uint indices[3] = { 0, 1, 2};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	std::string vertSrc = R"(
	#version 330 core

	layout(location=0) in vec3 aPos;

	out vec3 vPos;
	
	void main()
	{
		vPos = aPos + 0.5;
		gl_Position = vec4(aPos + 0.5, 1.0);
	}

	)";

	std::string fragSrc = R"(
	#version 330 core

	layout(location=0) out vec4 color;

	in vec3 vPos;

	void main()
	{
		color = vec4(vPos * 0.5 + 0.5, 1.0);
	}

	)";
	
	mShader.reset(new Shader(vertSrc, fragSrc));
}
//rebirth::Application::~Application() {}

void rebirth::Application::Run()
{
	
	while(mRunning)
	{
		glClearColor(0.05f, 0.05f, 0.05f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		mShader->Bind();
		glBindVertexArray(mVertexArray);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

		for(Layer* layer : mLayerStack)
		{
			layer->OnUpdate();
		}

		mImguiLayer->Begin();
		for(Layer* layer : mLayerStack)
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

	for(auto it = mLayerStack.end(); it != mLayerStack.begin();)
	{
		(*--it)->OnEvent(e);
		if(e.WasHandled())
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
