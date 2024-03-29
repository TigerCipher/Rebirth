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
// File Name: Components.h
// Date File Created: 6/27/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once


#include "rebirth/renderer/OrthoCamera.h"
#include "SceneCamera.h"
#include "rebirth/renderer/Texture.h"
#include "rebirth/core/UUID.h"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


namespace rebirth
{

	struct IDComponent
	{
		UUID uuid;
		IDComponent() = default;
		IDComponent(const IDComponent&) = default;

		const char* name = "IDComponent";
	};

	struct TagComponent
	{
		std::string tag;
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& pTag) : tag(pTag) {}

		const char* name = "TagComponent";
	};

	struct TransformComponent
	{
		glm::vec3 translation{ 0.0f };
		glm::vec3 rotation{ 0.0f };
		glm::vec3 scale{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& trans) : translation(trans) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rot = glm::toMat4(glm::quat(rotation));

			return glm::translate(glm::mat4(1.0f), translation) * rot * glm::scale(glm::mat4(1.0f), scale);
		}

		const char* name = "TransformComponent";
	};

	struct SpriteComponent
	{
		glm::vec4 color{ 1.0f };
		Ref<Texture2D> texture;
		float tilingFactor = 1.0f;


		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const glm::vec4& col) : color(col) {}

		const char* name = "SpriteComponent";
	};

	struct CircleComponent
	{
		glm::vec4 color{ 1.0f };
		float thickness = 1.0f; // filled in circle
		float fade = 0.005f;


		CircleComponent() = default;
		CircleComponent(const CircleComponent&) = default;

		const char* name = "CircleComponent";
	};


	struct CameraComponent
	{
		SceneCamera camera;
		bool primary = false;
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		const char* name = "CameraComponent";
	};

	class ScriptableEntity;

	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
		}

		const char* name = "NativeScriptComponent";
	};


	// Physics

	struct RigidBody2DComponent
	{
		enum class BodyType { STATIC = 0, DYNAMIC, KINEMATIC };
		BodyType bodyType = BodyType::STATIC;
		bool fixedRotation = false;

		void* runtimeBody = nullptr;

		RigidBody2DComponent() = default;
		RigidBody2DComponent(const RigidBody2DComponent&) = default;

		const char* name = "RigidBody2DComponent";
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 offset = { 0, 0 };
		glm::vec2 size = { 0.5f, 0.5f };

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;

		void* runtimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;

		const char* name = "BoxCollider2DComponent";
	};

	struct CircleCollider2DComponent
	{
		glm::vec2 offset = { 0, 0 };
		float radius = 0.5f;

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;

		void* runtimeFixture = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;

		const char* name = "CircleCollider2DComponent";
	};


	template<typename... C>
	struct ComponentGroup {};

	using AllComponents_NoID_NoTag = ComponentGroup
		<
		TransformComponent,
		SpriteComponent,
		CircleComponent,
		CameraComponent,
		NativeScriptComponent,
		RigidBody2DComponent,
		BoxCollider2DComponent,
		CircleCollider2DComponent
		>;

	using AllComponents = ComponentGroup
		<
		IDComponent,
		TagComponent,
		TransformComponent,
		SpriteComponent,
		CircleComponent,
		CameraComponent,
		NativeScriptComponent,
		RigidBody2DComponent,
		BoxCollider2DComponent,
		CircleCollider2DComponent
		>;
}

