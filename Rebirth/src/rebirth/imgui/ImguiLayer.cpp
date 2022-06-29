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
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "rebirth/core/Application.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace rebirth
{

	static void Style()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		/// 0 = FLAT APPEARENCE
		/// 1 = MORE "3D" LOOK
		float is3D = 1.0f;

		colors[ImGuiCol_Text] =						ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] =				ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_ChildBg] =					ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_WindowBg] =					ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_PopupBg] =					ImVec4(0.25f, 0.25f, 0.25f, 1.00f);

		colors[ImGuiCol_Border] =					ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
		colors[ImGuiCol_BorderShadow] =				ImVec4(1.00f, 1.00f, 1.00f, 0.06f);

		colors[ImGuiCol_FrameBg] =					ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] =			ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
		colors[ImGuiCol_FrameBgActive] =			ImVec4(0.56f, 0.56f, 0.56f, 0.67f);

		colors[ImGuiCol_TitleBg] =					ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
		colors[ImGuiCol_TitleBgActive] =			ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] =			ImVec4(0.17f, 0.17f, 0.17f, 0.90f);

		colors[ImGuiCol_MenuBarBg] =				ImVec4(0.335f, 0.335f, 0.335f, 1.000f);

		colors[ImGuiCol_ScrollbarBg] =				ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] =			ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] =		ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] =		ImVec4(0.76f, 0.76f, 0.76f, 1.00f);

		colors[ImGuiCol_CheckMark] =				ImVec4(0.65f, 0.65f, 0.65f, 1.00f);

		colors[ImGuiCol_SliderGrab] =				ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] =			ImVec4(0.64f, 0.64f, 0.64f, 1.00f);

		colors[ImGuiCol_Button] =					ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
		colors[ImGuiCol_ButtonHovered] =			ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
		colors[ImGuiCol_ButtonActive] =				ImVec4(0.76f, 0.76f, 0.76f, 1.00f);

		colors[ImGuiCol_Header] =					ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_HeaderHovered] =			ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
		colors[ImGuiCol_HeaderActive] =				ImVec4(0.76f, 0.76f, 0.76f, 0.77f);

		colors[ImGuiCol_Separator] =				ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
		colors[ImGuiCol_SeparatorHovered] =			ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
		colors[ImGuiCol_SeparatorActive] =			ImVec4(0.702f, 0.671f, 0.600f, 0.674f);

		colors[ImGuiCol_ResizeGrip] =				ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] =		ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] =			ImVec4(0.26f, 0.59f, 0.98f, 0.95f);

		colors[ImGuiCol_PlotLines] =				ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] =			ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] =			ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] =		ImVec4(1.00f, 0.60f, 0.00f, 1.00f);

		colors[ImGuiCol_TextSelectedBg] =			ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
		colors[ImGuiCol_ModalWindowDimBg] =			ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		colors[ImGuiCol_DragDropTarget] =			ImVec4(1.00f, 1.00f, 0.00f, 0.90f);

		colors[ImGuiCol_NavHighlight] =				ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] =	ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] =		ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

		style.PopupRounding = 3;

		style.WindowPadding = ImVec2(4, 4);
		style.FramePadding = ImVec2(4, 4);
		style.ItemSpacing = ImVec2(6, 2);

		style.ScrollbarSize = 18;

		style.WindowBorderSize = 1;
		style.ChildBorderSize = 1;
		style.PopupBorderSize = 1;
		style.FrameBorderSize = is3D;

		style.WindowRounding = 3;
		style.ChildRounding = 3;
		style.FrameRounding = 3;
		style.ScrollbarRounding = 2;
		style.GrabRounding = 3;


		style.TabBorderSize = is3D;
		style.TabRounding = 3;

		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		style.ScaleAllSizes(Application::Instance().GetWindow().GetHighDPIScaleFactor());

	}

	ImguiLayer::ImguiLayer() : Layer("ImguiLayer") {}


	void ImguiLayer::OnAttach()
	{
		RB_PROFILE_FUNC();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// #FIX: Right now this loads from the editor filepath - not good for sandbox
		// #TODO: Font size should be able to change via user preferences in the editor
		io.Fonts->AddFontFromFileTTF("assets/fonts/ruda/Ruda-Bold.ttf", Application::Instance().GetWindow().GetHighDPIScaleFactor() * 18.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/ruda/Ruda-Regular.ttf", Application::Instance().GetWindow().GetHighDPIScaleFactor() * 18.0f);


		//ImGui::StyleColorsDark();
		Style();

		Application& app = Application::Instance();
		
		auto window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

	}

	void ImguiLayer::OnDetach()
	{
		RB_PROFILE_FUNC();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImguiLayer::OnImguiRender()
	{
		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);
	}

	void ImguiLayer::OnEvent(Event& e)
	{
		if (mBlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.mHandled |= e.IsInCategory(EVENT_CATEGORY_MOUSE) & io.WantCaptureMouse;
			e.mHandled |= e.IsInCategory(EVENT_CATEGORY_KEYBOARD) & io.WantCaptureKeyboard;
		}
	}

	void ImguiLayer::Begin()
	{
		RB_PROFILE_FUNC();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImguiLayer::End()
	{
		RB_PROFILE_FUNC();
		Application& app = Application::Instance();

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backupContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backupContext);
		}
	}

}