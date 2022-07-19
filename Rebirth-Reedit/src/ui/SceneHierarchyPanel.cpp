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

		if (mContext)
		{
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
				if (ImGui::MenuItem("Add Entity"))
				{
					Entity e = mContext->CreateEntity("New Entity");
					RB_CORE_TRACE("Creating new entity with ID: {}", e.GetUUID());
				}

				ImGui::EndPopup();
			}
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
			// Draws UUID for debug purposes
			std::string uuid = std::to_string((uint64)entity.GetUUID());
			UIHelper::DrawTooltip(uuid.c_str());

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
			numComponentsAvailable += AddComponent<CircleComponent>(mSelectionContext, "Circle");
			numComponentsAvailable += AddComponent<RigidBody2DComponent>(mSelectionContext, "RigidBody 2D");
			numComponentsAvailable += AddComponent<BoxCollider2DComponent>(mSelectionContext, "Box Collider 2D");
			numComponentsAvailable += AddComponent<CircleCollider2DComponent>(mSelectionContext, "Circle Collider 2D");

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
				ImGui::Button("Texture", { 100.0f, 0.0f });
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = gAssetsPath / path;
						Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
						if (texture->IsLoaded())
							component.texture = texture;
						else
							RB_CLIENT_WARN("Unable to load texture {}", texturePath.filename().string());
					}
					ImGui::EndDragDropTarget();
				}

				// Tiling Factor
				ImGui::DragFloat("Tiling Factor", &component.tilingFactor, 0.1f, 0.0f, 100.0f);
			});

		DrawComponent<CircleComponent>("Circle", entity, [](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
				ImGui::DragFloat("Thickness", &component.thickness, 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &component.fade, 0.00025f, 0.0f, 3.0f);
			});


		DrawComponent<RigidBody2DComponent>("RigidBody 2D", entity, [](auto& component)
			{

				const char* bodyTypeStr[] = { "Static", "Dynamic", "Kinematic" };
				const char* currentBodyTypeStr = bodyTypeStr[(int)component.bodyType];
				if (ImGui::BeginCombo("Body Type", currentBodyTypeStr))
				{
					for (int i = 0; i < 3; i++)
					{
						bool isSelected = currentBodyTypeStr == bodyTypeStr[i];
						if (ImGui::Selectable(bodyTypeStr[i], isSelected))
						{
							currentBodyTypeStr = bodyTypeStr[i];
							component.bodyType = (RigidBody2DComponent::BodyType)i;
						}

						if (isSelected) ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				ImGui::Checkbox("Fixed Rotation", &component.fixedRotation);
			});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
				ImGui::DragFloat2("Size", glm::value_ptr(component.size));
				ImGui::DragFloat("Density", &component.density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution Threshold", &component.restitutionThreshold, 0.01f, 0.0f);
			});

		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& component)
			{
				ImGui::DragFloat2("Offset", glm::value_ptr(component.offset));
				ImGui::DragFloat("Radius", &component.radius, 0.1f, 0.1f);
				ImGui::DragFloat("Density", &component.density, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Friction", &component.friction, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
				ImGui::DragFloat("Restitution Threshold", &component.restitutionThreshold, 0.01f, 0.0f);
			});
	}

}

