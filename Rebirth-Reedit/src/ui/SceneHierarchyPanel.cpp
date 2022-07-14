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

#include "util/UIHelper.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace rebirth
{

	extern const std::filesystem::path gAssetsPath;

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		mContext = scene;
		mSelectionContext = {};
	}

	void SceneHierarchyPanel::OnImguiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		mContext->mRegistry.each([&](auto entity)
			{
				Entity e{ entity, mContext.get() };
				DrawEntityNode(e);
			});

		//if (sDirtyCameraFlag)
		//{
		//	mContext->mRegistry.each([&](auto ent)
		//		{
		//			Entity e{ ent, mContext.get() };
		//			if (e.HasComponent<CameraComponent>())
		//			{
		//				auto& cc = e.GetComponent<CameraComponent>();
		//				cc.primary = false;
		//			}
		//		});
		//}

		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0))
		{
			mSelectionContext = {};
		}

		// Context menu on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Add Entity"))
				mContext->CreateEntity("New Entity");

			ImGui::EndPopup();
		}
		ImGui::End();



		ImGui::Begin("Properties");

		if (mSelectionContext)
		{
			DrawComponents(mSelectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		mSelectionContext = entity;
	}

	template<typename T, typename Fn>
	static void DrawComponent(const std::string& label, Entity entity, Fn func)
	{
		if (entity.HasComponent<T>())
		{
			const ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_DefaultOpen
				| ImGuiTreeNodeFlags_AllowItemOverlap
				| ImGuiTreeNodeFlags_SpanAvailWidth
				| ImGuiTreeNodeFlags_FramePadding
				| ImGuiTreeNodeFlags_Framed;

			auto& component = entity.GetComponent<T>();
			ImVec2 regAvail = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();

			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeFlags, label.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(regAvail.x - lineHeight * 0.75f);
			if (ImGui::Button("-", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;
				ImGui::EndPopup();
			}

			if (open)
			{
				func(component);
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.RemoveComponent<T>();
			}
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ((mSelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_SpanAvailWidth;
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
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
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


	template<typename T>
	static int AddComponent(Entity& entity, const std::string& label)
	{
		if (!entity.HasComponent<T>())
		{
			if (ImGui::MenuItem(label.c_str()))
			{
				entity.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
			return 1;
		}
		return 0;
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1.0f);
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");
		if (ImGui::BeginPopup("AddComponent"))
		{
			int numComponentsAvailable = 0;
			numComponentsAvailable += AddComponent<CameraComponent>(mSelectionContext, "Camera");
			numComponentsAvailable += AddComponent<SpriteComponent>(mSelectionContext, "Sprite");

			if (numComponentsAvailable == 0)
			{
				if (ImGui::MenuItem("No Available Components"))
				{
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();
		

		DrawComponent<TransformComponent>("Transform", entity, [](auto& trans)
			{
				UIHelper::DrawFloat3Control("Translation", trans.translation);
				glm::vec3 rot = glm::degrees(trans.rotation);
				UIHelper::DrawFloat3Control("Rotation", rot);
				trans.rotation = glm::radians(rot);
				UIHelper::DrawFloat3Control("Scale", trans.scale, 1.0f);
			});


		DrawComponent<CameraComponent>("Camera", entity, [](auto& camComp)
			{
				auto& cam = camComp.camera;

				// #TODO If checked, make other cameras go to not primary
				if (ImGui::Checkbox("Primary", &camComp.primary))
				{
					//sDirtyCameraFlag = camComp.primary;
				}

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
			});

		DrawComponent<SpriteComponent>("Sprite", entity, [](auto& component)
			{
				// Tint Color
				ImGui::ColorEdit4("Color", glm::value_ptr(component.color));

				// Texture
				ImGui::Button("Texture", {100.0f, 0.0f});
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = gAssetsPath / path;
						component.texture = Texture2D::Create(texturePath.string());
					}
					ImGui::EndDragDropTarget();
				}

				// Tiling Factor
				ImGui::DragFloat("Tiling Factor", &component.tilingFactor, 0.1f, 0.0f, 100.0f);
			});
	}

}

