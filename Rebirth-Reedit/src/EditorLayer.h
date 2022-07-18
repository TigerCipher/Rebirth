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
// File Name: EditorLayer.h
// Date File Created: 6/26/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include <Rebirth.h>
#include <rebirth/imgui/EditorConsolePanel.h>

#include "ui/SceneHierarchyPanel.h"
#include "ui/ContentBrowserPanel.h"

namespace rebirth
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer() : Layer("EditorLayer"), mCameraController(1920.0f / 1080.0f, true) {}
		virtual ~EditorLayer() = default;
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Timestep ts) override;
		void OnOverlayRender();
		void OnEvent(Event& e) override;
		void OnImguiRender() override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();
		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);

		void OnScenePlay();
		void OnSceneStop();

		void OnDuplicateEntity();

		void Viewport();
		void Toolbar();
		void Settings();

	private:
		OrthoCameraController mCameraController;
		bool mViewportFocused = false;
		bool mViewportHovered = false;


		// abstract to renderer
		Ref<Shader> mShader;
		Ref<VertexArray> mSquareVtxArray;

		Ref<Scene> mActiveScene;
		//Ref<Scene> mRuntimeScene;
		Ref<Scene> mEditorScene;

		std::filesystem::path mEditorScenePath;


		Entity mSquareEntity;
		Entity mCameraEntity;
		Entity mSecondCamera;
		bool primCam = true;

		glm::vec2 mViewportSize{0.0f};
		glm::vec2 mViewportBounds[2];

		Ref<Texture2D> mTexture;

		Ref<Framebuffer> mFramebuffer;

		EditorCamera mEditorCamera;
		Entity mHoveredEntity;

		int mGizmoType = -1;
		SceneHierarchyPanel mSceneHierarchyPanel;
		ContentBrowserPanel mContentBrowserPanel;
		EditorConsolePanel mConsolePanel;


		enum class SceneState
		{
			EDIT = 0,
			PLAY
		};

		SceneState mSceneState = SceneState::EDIT;

		Ref<Texture2D> mIconPlay;
		Ref<Texture2D> mIconStop;
		bool mShowPhysicsColliders = false;
	};
}