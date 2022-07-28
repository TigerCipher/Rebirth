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

	extern const std::filesystem::path gAssetsPath;

	void EditorLayer::OnAttach()
	{
		RB_PROFILE_FUNC();
		mTexture = Texture2D::Create("assets/textures/default.png");
		mIconPlay = Texture2D::Create("assets/icons/play_button.png");
		mIconStop = Texture2D::Create("assets/icons/stop_button.png");
		mIconSimulate = Texture2D::Create("assets/icons/simulate_button.png");

		FramebufferDesc spec;
		spec.attachements = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INT, FramebufferTextureFormat::DEPTH };
		spec.width = Application::Instance().GetWindow().GetWidth();
		spec.height = Application::Instance().GetWindow().GetHeight();
		mFramebuffer = Framebuffer::Create(spec);

		mEditorScene = createRef<Scene>();
		mActiveScene = mEditorScene;

		if (const auto cmd = Application::Instance().GetCommandLineArgs(); cmd.count > 1)
		{
			if (const auto scenePath = cmd[1]; std::filesystem::exists(scenePath))
			{
				SceneSerializer serializer(mActiveScene);
				serializer.DeserializeFromYaml(scenePath);
				mEditorScene = mActiveScene;
				mEditorScenePath = scenePath;
			}
		}


		mEditorCamera = EditorCamera(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
		mSceneHierarchyPanel.SetContext(mActiveScene);

		Renderer2D::SetLineWidth(6.0f);
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		RB_PROFILE_FUNC();

		{
			RB_PROFILE_SCOPE("Viewport check");
			if (FramebufferDesc spec = mFramebuffer->GetDesc();
				mViewportSize.x > 0.0f && mViewportSize.y > 0.0f &&
				(spec.width != mViewportSize.x || spec.height != mViewportSize.y))
			{
				mFramebuffer->Resize((uint32)mViewportSize.x, (uint32)mViewportSize.y);
				//mCameraController.ResizeBounds(mViewportSize.x, mViewportSize.y);
				mEditorCamera.SetViewportSize(mViewportSize.x, mViewportSize.y);
				mActiveScene->OnViewportResize((uint32)mViewportSize.x, (uint32)mViewportSize.y);
			}
		}

		Renderer2D::ResetStats();

		mFramebuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		mFramebuffer->ClearAttachment(1, -1);


		switch (mSceneState)
		{
			case SceneState::EDIT:
			{
				RB_PROFILE_SCOPE("Update EDIT");
				mEditorCamera.OnUpdate(ts);
				mActiveScene->OnUpdateEditor(ts, mEditorCamera);
				break;
			}
			case SceneState::PLAY:
			{
				RB_PROFILE_SCOPE("Update PLAY");
				mActiveScene->OnUpdateRuntime(ts);
				break;
			}
			case SceneState::SIMULATE:
			{
				RB_PROFILE_SCOPE("Update STOP");
				mActiveScene->OnUpdateSimulation(ts, mEditorCamera);
				break;
			}
		}


		{
			RB_PROFILE_SCOPE("Mouse Picking");
			auto [mx, my] = ImGui::GetMousePos();
			mx -= mViewportBounds[0].x;
			my -= mViewportBounds[0].y;

			glm::vec2 viewportSize = mViewportBounds[1] - mViewportBounds[0];
			my = viewportSize.y - my;

			int mouseX = (int)mx;
			int mouseY = (int)my;

			if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
			{
				int pixelData = mFramebuffer->ReadPixel(1, mouseX, mouseY);
				if (pixelData == -1)
					mHoveredEntity = Entity();
				else mHoveredEntity = Entity((entt::entity)pixelData, mActiveScene.get());
			}
		}

		OnOverlayRender();
		mFramebuffer->Unbind();

	}

	void EditorLayer::OnOverlayRender()
	{
		RB_PROFILE_FUNC();
		if (mSceneState == SceneState::PLAY)
		{
			Entity cam = mActiveScene->GetPrimaryCameraEntity();
			if (!cam) return;
			Renderer2D::BeginScene(cam.GetComponent<CameraComponent>().camera, cam.GetComponent<TransformComponent>().GetTransform());
		}
		else
		{
			Renderer2D::BeginScene(mEditorCamera);
		}

		if (mShowPhysicsColliders)
		{
			{
				auto view = mActiveScene->GetAllEntities<TransformComponent, BoxCollider2DComponent>();
				for (auto e : view)
				{
					auto [tc, bc] = view.get<TransformComponent, BoxCollider2DComponent>(e);

					glm::vec3 trans = tc.translation + glm::vec3(bc.offset, 0.001f);
					glm::vec3 scale = tc.scale * glm::vec3(bc.size * 2.0f, 1.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), trans) *
						glm::rotate(glm::mat4(1.0f), tc.rotation.z, { 0, 0, 1 }) *
						glm::scale(glm::mat4(1.0f), scale);
					Renderer2D::DrawRect(transform, { 0, 1, 0, 1 });
				}
			}

			{
				auto view = mActiveScene->GetAllEntities<TransformComponent, CircleCollider2DComponent>();
				for (auto e : view)
				{
					auto [tc, cc] = view.get<TransformComponent, CircleCollider2DComponent>(e);

					glm::vec3 trans = tc.translation + glm::vec3(cc.offset, 0.001f);
					glm::vec3 scale = tc.scale * glm::vec3(cc.radius * 2.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), trans) *
						glm::scale(glm::mat4(1.0f), scale);
					Renderer2D::DrawCircle(transform, Colors::GREEN, 0.01f);
				}
			}
		}

		if (Entity selected = mSceneHierarchyPanel.GetSelectedEntity())
		{
			const auto& tc = selected.GetComponent<TransformComponent>();
			glm::vec3 trans = tc.translation + glm::vec3(0, 0, 0.002f);
			glm::vec3 scale = tc.scale;
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), trans) *
				glm::rotate(glm::mat4(1.0f), tc.rotation.z, { 0, 0, 1 }) *
				glm::scale(glm::mat4(1.0f), scale);
			Renderer2D::DrawRect(transform, Colors::YELLOW);
		}


		Renderer2D::EndScene();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		//Panels::sConsolePanel->OnEvent(e);
		//mCameraController.OnEvent(e);

		if (mSceneState == SceneState::EDIT)
		{
			mEditorCamera.OnEvent(e);
		}

		if (e.GetType() == EventType::KEY_PRESSED)
			OnKeyPressed((KeyPressedEvent&)e);
		else if (e.GetType() == EventType::MOUSE_BUTTON_PRESSED)
			OnMouseButtonPressed((MouseButtonPressedEvent&)e);
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

				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					SaveScene();
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
		mContentBrowserPanel.OnImguiRender();
		if(Panels::sConsolePanel)
			Panels::sConsolePanel->OnImguiRender();


		ImGui::Begin("Statistics");

		std::string name = "None";
		if (mHoveredEntity)
			name = mHoveredEntity.GetComponent<TagComponent>().tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Render Batch Stats:");
		ImGui::Text("Draw Calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quads);
		ImGui::Text("Vertex Count: %d", stats.GetVertCount());
		ImGui::Text("Index Count: %d", stats.GetIndicesCount());

		for (int i = 0; i < 6; i++)
		{
			ImGui::Separator();
		}
		ImGui::Text("Performance Stats:");
		ImGui::Text("Frametime (seconds): %f", Statistics::GetRenderStats().frameTime);
		ImGui::Text("Frametime (milliseconds): %f", Statistics::GetRenderStats().frameTime * 1000.0f);
		ImGui::Text("FPS: %d", Statistics::GetRenderStats().fps);
		bool isVsyncOn = Application::Instance().GetWindow().IsVSync();
		const char* vsyncStatus = isVsyncOn ? "ENABLED" : "DISABLED";
		ImGui::Text("VSync Status: %s", vsyncStatus);
		if (ImGui::Button("Toggle VSync"))
		{
			Application::Instance().GetWindow().SetVSync(!isVsyncOn);
		}

		for (int i = 0; i < 6; i++)
		{
			ImGui::Separator();
		}

		ImGui::Text("Renderer Information:");
		ImGui::Text("Vendor: %s", GraphicsAPI::GetCapabilities().vendor.c_str());
		ImGui::Text("Renderer: %s", GraphicsAPI::GetCapabilities().renderer.c_str());
		ImGui::Text("Version: %s", GraphicsAPI::GetCapabilities().version.c_str());
		ImGui::Text("Max Samples: %d", GraphicsAPI::GetCapabilities().maxSamples);
		ImGui::Text("Max Aniostropy: %f", GraphicsAPI::GetCapabilities().maxAniostropy);

		ImGui::End();
#ifdef RB_SHOW_IMGUI_DEMO
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
#endif
		Settings();
		Viewport();
		Toolbar();

		ImGui::End();

	}


	void EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == MouseButton::LEFT)
		{
			if (mViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(KeyCode::LEFT_ALT))
				mSceneHierarchyPanel.SetSelectedEntity(mHoveredEntity);
		}

	}


	void EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Non-shortcut keybinds

		// Shortcuts
		if (e.GetRepeatCount() > 0) return;

		// modifiers
		bool control = Input::IsKeyPressed(KeyCode::LEFT_CONTROL) || Input::IsKeyPressed(KeyCode::RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(KeyCode::LEFT_SHIFT) || Input::IsKeyPressed(KeyCode::RIGHT_SHIFT);

		switch (e.GetKeyCode())
		{
			case KeyCode::N:
				if (control)
				{
					NewScene();
				}
				break;
			case KeyCode::O:
				if (control)
				{
					OpenScene();
				}
				break;
			case KeyCode::S:
				if (control)
				{
					if (shift)
						SaveSceneAs();
					else SaveScene();
				}
				break;

			case KeyCode::Q:
				mGizmoType = -1;
				break;
			case KeyCode::W:
				mGizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case KeyCode::E:
				mGizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case KeyCode::R:
				mGizmoType = ImGuizmo::OPERATION::SCALE;
				break;

			case KeyCode::D:
				if (control)
					OnDuplicateEntity();
				break;

			default: break;
		}

	}

	void EditorLayer::NewScene()
	{
		mActiveScene = createRef<Scene>();
		mActiveScene->OnViewportResize((uint32)mViewportSize.x, (uint32)mViewportSize.y);
		mSceneHierarchyPanel.SetContext(mActiveScene);
		mEditorScene = mActiveScene;
		mEditorScenePath = std::filesystem::path();
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialog::OpenFile("Rebirth Scene (*.rebirth)\0*.rebirth\0");
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (mSceneState != SceneState::EDIT)
			OnSceneStop();
		if (path.extension().string() != ".rebirth")
		{
			RB_CLIENT_WARN("{} is not a scene file", path.filename().string());
			return;
		}

		Ref<Scene> newScene = createRef<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.DeserializeFromYaml(path.string()))
		{
			mEditorScene = newScene;
			mEditorScene->OnViewportResize((uint32)mViewportSize.x, (uint32)mViewportSize.y);
			mSceneHierarchyPanel.SetContext(mEditorScene);
			mActiveScene = mEditorScene;
			mEditorScenePath = path;
		}
	}

	void EditorLayer::SaveScene()
	{
		if (mEditorScenePath.empty())
		{
			mEditorScenePath = "assets/scenes/untitled.rebirth";
		}

		SerializeScene(mActiveScene, mEditorScenePath);
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialog::SaveFile("Rebirth Scene (*.rebirth)\0*.rebirth\0");
		if (!filepath.empty())
		{
			SerializeScene(mActiveScene, filepath);
			mEditorScenePath = filepath;
		}
	}

	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneSerializer serializer(scene);
		serializer.SerializeToYaml(path.string());
	}

	void EditorLayer::OnScenePlay()
	{
		if (mSceneState == SceneState::SIMULATE)
			OnSceneStop();
		mSceneState = SceneState::PLAY;

		mActiveScene = Scene::Copy(mEditorScene);

		Application::Instance().HandleEvents(new ScenePreStartEvent(mActiveScene));
		mActiveScene->OnRuntimeStart();
		Application::Instance().HandleEvents(new ScenePostStartEvent(mActiveScene));

		mSceneHierarchyPanel.SetContext(mActiveScene);
	}

	void EditorLayer::OnSceneStop()
	{
		mSceneState = SceneState::EDIT;

		if (mSceneState == SceneState::PLAY)
		{
			Application::Instance().HandleEvents(new ScenePreStopEvent(mActiveScene));
			mActiveScene->OnRuntimeStop();
			Application::Instance().HandleEvents(new ScenePostStopEvent(mActiveScene));
		}
		else if (mSceneState == SceneState::SIMULATE)
			mActiveScene->OnSimulationStop();


		mActiveScene = mEditorScene;

		mSceneHierarchyPanel.SetContext(mActiveScene);
	}

	void EditorLayer::OnSceneSimulate()
	{
		if (mSceneState == SceneState::PLAY)
			OnSceneStop();
		mSceneState = SceneState::SIMULATE;
		mActiveScene = Scene::Copy(mEditorScene);
		mActiveScene->OnSimulationStart();

		mSceneHierarchyPanel.SetContext(mActiveScene);
	}

	void EditorLayer::OnDuplicateEntity()
	{
		if (mSceneState != SceneState::EDIT) return;
		if (Entity e = mSceneHierarchyPanel.GetSelectedEntity())
		{
			mEditorScene->DuplicateEntity(e);
		}
	}

	void EditorLayer::Viewport()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		mViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		mViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		mViewportFocused = ImGui::IsWindowFocused();
		mViewportHovered = ImGui::IsWindowHovered();

		if (!ImGui::IsAnyItemActive())
			Application::Instance().GetImguiLayer()->SetBlockEvents(!mViewportFocused && !mViewportHovered);
		else
			Application::Instance().GetImguiLayer()->SetBlockEvents(!mViewportFocused || !mViewportHovered);

		ImVec2 viewPanelSize = ImGui::GetContentRegionAvail();

		mViewportSize = { viewPanelSize.x, viewPanelSize.y };

		uint32 textureID = mFramebuffer->GetColorAttachmentID();
		ImGui::Image((void*)(uint64)textureID, ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(gAssetsPath / path);
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmo smizmos
		Entity selectedEntity = mSceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && mGizmoType != -1 && !Input::IsKeyPressed(KeyCode::LEFT_ALT))
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(mViewportBounds[0].x, mViewportBounds[0].y,
				mViewportBounds[1].x - mViewportBounds[0].x,
				mViewportBounds[1].y - mViewportBounds[0].y);


			//editor
			const glm::mat4& camProj = mEditorCamera.GetProjection();
			glm::mat4 camView = mEditorCamera.GetViewMatrix();

			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			bool snap = Input::IsKeyPressed(KeyCode::LEFT_CONTROL);
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
	}

	void EditorLayer::Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 2 });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0, 0 });
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		const auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.25f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		bool toolbarEnabled = (bool)mActiveScene;
		ImVec4 tint = ImVec4(1, 1, 1, 1);
		if (!toolbarEnabled)
			tint.w = 0.5f;

		float size = ImGui::GetWindowHeight() - 4.0f;
		{
			Ref<Texture2D> icon = (mSceneState == SceneState::EDIT || mSceneState == SceneState::SIMULATE) ? mIconPlay : mIconStop;
			ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x * 0.5f) - (size * 0.5f));

			if (ImGui::ImageButton((ImTextureID)icon->GetId(), { size, size }, { 0, 0 }, { 1, 1 }, 0, {0, 0, 0, 0}, tint) && toolbarEnabled)
			{
				if (mSceneState == SceneState::EDIT || mSceneState == SceneState::SIMULATE)
					OnScenePlay();
				else if (mSceneState == SceneState::PLAY)
					OnSceneStop();
			}
		}

		ImGui::SameLine();

		{
			Ref<Texture2D> icon = (mSceneState == SceneState::EDIT || mSceneState == SceneState::PLAY) ? mIconSimulate : mIconStop;
			//ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x * 0.5f) - (size * 0.5f));

			if (ImGui::ImageButton((ImTextureID)icon->GetId(), { size, size }, { 0, 0 }, { 1, 1 }, 0, {0, 0, 0, 0}, tint) && toolbarEnabled)
			{
				if (mSceneState == SceneState::EDIT || mSceneState == SceneState::PLAY)
					OnSceneSimulate();
				else if (mSceneState == SceneState::SIMULATE)
					OnSceneStop();
			}
		}


		ImGui::End(); // ##toolbar

		ImGui::PopStyleColor(5);
		ImGui::PopStyleVar(2);
	}

	void EditorLayer::Settings()
	{
		ImGui::Begin("Settings");

		ImGui::Checkbox("Show Physics Colliders", &mShowPhysicsColliders);

		ImGui::End(); // Settings
	}

}
