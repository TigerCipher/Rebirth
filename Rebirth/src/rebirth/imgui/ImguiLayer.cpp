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
// File Name: ImguiLayer.cpp
// Date File Created: 06/17/2022 at 10:17 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------



#include "rbpch.h"

#include "ImguiLayer.h"

#include "imgui.h"
#include "platform/opengl/ImguiImpl.h"

#include "rebirth/Application.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

rebirth::ImguiLayer::ImguiLayer(): Layer("ImguiLayer") {}


void rebirth::ImguiLayer::OnAttach()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	
	
	ImGui::StyleColorsDark();

	Application& app = Application::Instance();
	GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
	
}

void rebirth::ImguiLayer::OnDetach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void rebirth::ImguiLayer::OnUpdate()
{
	Application& app = Application::Instance();

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
	

	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	static bool show = true;
	ImGui::ShowDemoWindow(&show);

	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void rebirth::ImguiLayer::OnEvent(Event& e) {}