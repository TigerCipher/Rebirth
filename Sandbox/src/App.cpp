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

class SampleLayer : public rebirth::Layer
{
public:
	SampleLayer() : Layer("Sample") {}

	void OnUpdate() override
	{
		//RB_CLIENT_INFO("SampleLayer Update");
	}

	void OnEvent(rebirth::Event& e) override
	{
		//RB_CLIENT_TRACE("{0}", e);
	}
};

class Sandbox : public rebirth::Application
{
public:
	Sandbox()
	{
		PushLayer(new SampleLayer());
		PushOverlay(new rebirth::ImguiLayer());
	}
	~Sandbox() override = default;
	
};


rebirth::Application* rebirth::CreateApplication()
{
	int a = 3;
	RB_CLIENT_INFO("Starting client A={0}", a);
	return new Sandbox();
}
