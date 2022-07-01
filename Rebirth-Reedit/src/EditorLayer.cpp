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
// File Name: EditorLayer.cpp
// Date File Created: 6/26/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>

namespace rebirth
{
	void EditorLayer::OnAttach()
	{



		RB_PROFILE_FUNC();
		mTexture = Texture2D::Create("assets/textures/default.png");

		FramebufferDesc spec;
		spec.width = Application::Instance().GetWindow().GetWidth();
		spec.height = Application::Instance().GetWindow().GetHeight();
		mFramebuffer = Framebuffer::Create(spec);

		mActiveScene = createRef<Scene>();
#if 0
		mSquareEntity = mActiveScene->CreateEntity("Square");
		mSquareEntity.AddComponent<SpriteComponent>(glm::vec4{ 0.3f, 0.85f, 0.4f, 1.0f });

		auto sq2 = mActiveScene->CreateEntity("Red Square");
		sq2.AddComponent<SpriteComponent>(glm::vec4{ 0.85f, 0.25f, 0.2f, 1.0f });


		mCameraEntity = mActiveScene->CreateEntity("Camera");
		auto& c = mCameraEntity.AddComponent<CameraComponent>();
		c.primary = true;

		mSecondCamera = mActiveScene->CreateEntity("Camera 2");
		mSecondCamera.AddComponent<CameraComponent>();

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				auto& transform = GetComponent<TransformComponent>();
				transform.translation.x = rand() % 10 - 5.0f;
			}

			void OnDestroy()
			{

			}

			void OnUpdate(Timestep ts)
			{
				auto& translation = GetComponent<TransformComponent>().translation;
				static float speed = 5.0f;
				if (Input::IsKeyPressed(RB_KEY_A))
				{
					translation.x -= speed * ts;
				}

				if (Input::IsKeyPressed(RB_KEY_D))
				{
					translation.x += speed * ts;
				}

				if (Input::IsKeyPressed(RB_KEY_W))
				{
					translation.y += speed * ts;
				}

				if (Input::IsKeyPressed(RB_KEY_S))
				{
					translation.y -= speed * ts;
				}
			}
		};

		mCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		mSecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

		mSceneHierarchyPanel.SetContext(mActiveScene);

	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		RB_PROFILE_FUNC();

		if (FramebufferDesc spec = mFramebuffer->GetSpecification();
			mViewportSize.x > 0.0f && mViewportSize.y > 0.0f &&
			(spec.width != mViewportSize.x || spec.height != mViewportSize.y))
		{
			mFramebuffer->Resize((uint32)mViewportSize.x, (uint32)mViewportSize.y);
			mCameraController.ResizeBounds(mViewportSize.x, mViewportSize.y);

			mActiveScene->OnViewportResize((uint32)mViewportSize.x, (uint32)mViewportSize.y);
		}

		if (mViewportFocused)
		{
			mCameraController.OnUpdate(ts);
		}


		Renderer2D::ResetStats();

		mFramebuffer->Bind();

		RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1.0f });
		RenderCommand::Clear();


		//Renderer2D::BeginScene(mCameraController.GetCamera());
		mActiveScene->OnUpdate(ts);

		//Renderer2D::EndScene();

		mFramebuffer->Unbind();

	}

	void EditorLayer::OnEvent(Event& e)
	{
		mCameraController.OnEvent(e);

		EventDispatcher disp(e);
		disp.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));
	}

	void EditorLayer::OnImguiRender()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSize;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();

				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}

				if (ImGui::MenuItem("Exit")) Application::Instance().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}


		mSceneHierarchyPanel.OnImguiRender();


		ImGui::Begin("Statistics");
		auto stats = Renderer2D::GetStats();
		ImGui::Text("Render Batch Stats:");
		ImGui::Text("Draw Calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quads);
		ImGui::Text("Vertex Count: %d", stats.GetVertCount());
		ImGui::Text("Index Count: %d", stats.GetIndicesCount());
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		mViewportFocused = ImGui::IsWindowFocused();
		mViewportHovered = ImGui::IsWindowHovered();

		if(!ImGui::IsAnyItemActive())
			Application::Instance().GetImguiLayer()->SetBlockEvents(!mViewportFocused && !mViewportHovered);
		else
			Application::Instance().GetImguiLayer()->SetBlockEvents(!mViewportFocused || !mViewportHovered);

		ImVec2 viewPanelSize = ImGui::GetContentRegionAvail();

		mViewportSize = { viewPanelSize.x, viewPanelSize.y };

		uint32 textureID = mFramebuffer->GetColorAttachmentID();
		ImGui::Image((void*)(uint64)textureID, ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		Entity selectedEntity = mSceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && mGizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float winWidth = (float)ImGui::GetWindowWidth();
			float winHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, winWidth, winHeight);

			auto camEntity = mActiveScene->GetPrimaryCameraEntity();
			const auto& cam = camEntity.GetComponent<CameraComponent>().camera;
			const glm::mat4& camProj = cam.GetProjection();
			glm::mat4 camView = glm::inverse(camEntity.GetComponent<TransformComponent>().GetTransform());

			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			bool snap = Input::IsKeyPressed(RB_KEY_LEFT_ALT);
			float snapValue = mGizmoType == ImGuizmo::OPERATION::ROTATE ? 45.0f : mGizmoType == ImGuizmo::OPERATION::SCALE ? 0.25f : 0.5f; // 45 for rotation, 0.25 for scale, 0.5 for translation

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(camView), glm::value_ptr(camProj), (ImGuizmo::OPERATION)mGizmoType,
				ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::Decompose(transform, translation, rotation, scale);
				glm::vec3 deltaRot = rotation - tc.rotation;
				tc.translation = translation;
				tc.rotation += deltaRot;
				tc.scale = scale;
			}

		}



		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();

	}


	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Non-shortcut keybinds

		// Shortcuts
		if (e.GetRepeatCount() > 0) return false;

		// modifiers
		bool control = Input::IsKeyPressed(RB_KEY_LEFT_CONTROL) || Input::IsKeyPressed(RB_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(RB_KEY_LEFT_SHIFT) || Input::IsKeyPressed(RB_KEY_RIGHT_SHIFT);

		switch (e.GetKeyCode())
		{
			case RB_KEY_N:
				if (control)
				{
					NewScene();
				}
				break;
			case RB_KEY_O:
				if (control)
				{
					OpenScene();
				}
				break;
			case RB_KEY_S:
				if (control && shift)
				{
					SaveSceneAs();
				}
				break;

			case RB_KEY_Q:
				mGizmoType = -1;
				break;
			case RB_KEY_W:
				mGizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case RB_KEY_E:
				mGizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case RB_KEY_R:
				mGizmoType = ImGuizmo::OPERATION::SCALE;
				break;

			default: break;
		}
	}

	void EditorLayer::NewScene()
	{
		mActiveScene = createRef<Scene>();
		mActiveScene->OnViewportResize((uint32)mViewportSize.x, (uint32)mViewportSize.y);
		mSceneHierarchyPanel.SetContext(mActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialog::OpenFile("Rebirth Scene (*.rebirth)\0*.rebirth\0");
		if (!filepath.empty())
		{
			//mActiveScene->DestroyAll();
			mActiveScene = createRef<Scene>();
			mActiveScene->OnViewportResize((uint32)mViewportSize.x, (uint32)mViewportSize.y);
			mSceneHierarchyPanel.SetContext(mActiveScene);

			SceneSerializer serializer(mActiveScene);
			serializer.DeserializeFromYaml(filepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialog::SaveFile("Rebirth Scene (*.rebirth)\0*.rebirth\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(mActiveScene);
			serializer.SerializeToYaml(filepath);
		}
	}
}
