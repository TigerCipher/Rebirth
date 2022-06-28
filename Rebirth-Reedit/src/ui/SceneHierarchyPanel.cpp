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
// File Name: SceneHierarchyPanel.cpp
// Date File Created: 6/28/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "SceneHierarchyPanel.h"

#include "../util/UIHelper.h"

#include <imgui/imgui.h>

namespace rebirth
{

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		mContext = scene;
	}

	void SceneHierarchyPanel::OnImguiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		mContext->mRegistry.each([&](auto entity)
			{
				Entity e{ entity, mContext.get() };
				DrawEntityNode(e);
			});

		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0))
		{
			mSelectionContext = {};
		}

		// Context menu on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if(ImGui::MenuItem("Add Entity"))
				mContext->CreateEntity("New Entity");

			ImGui::EndPopup();
		}
		ImGui::End();



		ImGui::Begin("Properties");

		if (mSelectionContext)
		{
			DrawComponents(mSelectionContext);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");
			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					mSelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite"))
				{
					mSelectionContext.AddComponent<SpriteComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ((mSelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool expanded = ImGui::TreeNodeEx((void*)(uint64)(uint32)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			mSelectionContext = entity;
		}

		bool deleteEntity = false;
		// Context menu on entity
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				deleteEntity = true;

			ImGui::EndPopup();
		}

		if (expanded)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool expanded = ImGui::TreeNodeEx((void*)(3456), flags, tag.c_str());
			if (expanded) ImGui::TreePop();
			ImGui::TreePop();
		}

		if (deleteEntity)
		{
			mContext->DestroyEntity(entity);
			if (mSelectionContext == entity)
				mSelectionContext = {};
		}

	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}
		const ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<TransformComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeFlags, "Transform");


			if(open)
			{
				auto& trans = entity.GetComponent<TransformComponent>();
				UIHelper::DrawFloat3Control("Translation", trans.translation);
				glm::vec3 rot = glm::degrees(trans.rotation);
				UIHelper::DrawFloat3Control("Rotation", rot);
				trans.rotation = glm::radians(rot);
				UIHelper::DrawFloat3Control("Scale", trans.scale, 1.0f);
				ImGui::TreePop();
			}


		}


		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeFlags, "Transform"))
			{
				auto& camComp = entity.GetComponent<CameraComponent>();
				auto& cam = camComp.camera;

				ImGui::Checkbox("Primary", &camComp.primary);

				const char* projStr[] = { "Perspective", "Orthographic" };
				const char* currentProjStr = projStr[(int)cam.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjStr))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjStr == projStr[i];
						if (ImGui::Selectable(projStr[i], isSelected))
						{
							currentProjStr = projStr[i];
							cam.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected) ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (cam.GetProjectionType() == SceneCamera::ProjectionType::PERSPECTIVE)
				{
					float fov = glm::degrees(cam.GetPerspectiveFoV());
					if (ImGui::DragFloat("FOV", &fov))
						cam.SetPerspectiveFoV(glm::radians(fov));

					float nearClip = cam.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near Clip", &nearClip))
						cam.SetPerspectiveNearClip(nearClip);

					float farClip = cam.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far Clip", &farClip))
						cam.SetPerspectiveFarClip(farClip);
				}

				if (cam.GetProjectionType() == SceneCamera::ProjectionType::ORTHOGRAPHIC)
				{
					float size = cam.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &size))
						cam.SetOrthographicSize(size);

					float nearClip = cam.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near Clip", &nearClip))
						cam.SetOrthographicNearClip(nearClip);

					float farClip = cam.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far Clip", &farClip))
						cam.SetOrthographicFarClip(farClip);

					ImGui::Checkbox("Fixed Aspect Ratio", &camComp.fixedAspectRatio);
				}

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<SpriteComponent>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			bool open = ImGui::TreeNodeEx((void*)typeid(SpriteComponent).hash_code(), treeFlags, "Sprite");
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("+", ImVec2{20, 20}))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleVar();
			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;
				ImGui::EndPopup();
			}

			if (open)
			{
				auto& spriteComp = entity.GetComponent<SpriteComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(spriteComp.color));
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.RemoveComponent<SpriteComponent>();
			}
		}
	}

}

