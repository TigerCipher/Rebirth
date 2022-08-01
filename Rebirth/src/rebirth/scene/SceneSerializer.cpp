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
// File Name: SceneSerializer.cpp
// Date File Created: 6/29/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "SceneSerializer.h"

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

#include "Entity.h"
#include "Components.h"

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace rebirth
{

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static std::string BodyTypeToString(RigidBody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case RigidBody2DComponent::BodyType::STATIC: return "Static";
			case RigidBody2DComponent::BodyType::DYNAMIC: return "Dynamic";
			case RigidBody2DComponent::BodyType::KINEMATIC: return "Kinematic";
		}

		RB_CORE_ASSERT(false, "Unknown rigid body type");
		return {};
	}

	static RigidBody2DComponent::BodyType BodyTypeFromString(const std::string& bodyType)
	{
		if (bodyType == "Static") return RigidBody2DComponent::BodyType::STATIC;
		if (bodyType == "Dynamic") return RigidBody2DComponent::BodyType::DYNAMIC;
		if (bodyType == "Kinematic") return RigidBody2DComponent::BodyType::KINEMATIC;
		RB_CORE_ASSERT(false, "Unknown rigid body type");
		return RigidBody2DComponent::BodyType::STATIC;
	}

	// Serialize Components

	template<typename... T>
	static void SerializeAllComponents(YAML::Emitter& out, Entity entity)
	{
		([&]()
			{
				if (entity.HasComponent<T>())
				{
					const T& component = entity.GetComponent<T>();
					RB_CORE_INFO("Serializing {} for entity {}", component.name, entity.GetUUID());
					SerializeComponent<T>(out, entity, component);
				}
			}(), ...);

	}

	template<typename... T>
	static void SerializeAllComponents(ComponentGroup<T...>, YAML::Emitter& out, Entity entity)
	{
		SerializeAllComponents<T...>(out, entity);
	}

	// Not sure I like this design. Might be better to just go back to if statements for components, but this makes it easier to navigate and find them via VA Outline
	template<typename C>
	static void SerializeComponent(YAML::Emitter& out, Entity enitty, const C& component) {}

	template<>
	static void SerializeComponent<TagComponent>(YAML::Emitter& out, Entity entity, const TagComponent& component)
	{
		out << YAML::Key << "TagComponent";
		out << YAML::BeginMap; // TagComponent

		auto& tag = component.tag;
		out << YAML::Key << "Tag" << YAML::Value << tag;

		out << YAML::EndMap; // TagComponent
	}

	template<>
	static void SerializeComponent<TransformComponent>(YAML::Emitter& out, Entity entity, const TransformComponent& component)
	{
		out << YAML::Key << "TransformComponent";
		out << YAML::BeginMap; // TransformComponent
		out << YAML::Key << "Translation" << YAML::Value << component.translation;
		out << YAML::Key << "Rotation" << YAML::Value << component.rotation;
		out << YAML::Key << "Scale" << YAML::Value << component.scale;

		out << YAML::EndMap; // TransformComponent
	}

	template<>
	static void SerializeComponent<CameraComponent>(YAML::Emitter& out, Entity entity, const CameraComponent& component)
	{
		out << YAML::Key << "CameraComponent";
		out << YAML::BeginMap; // CameraComponent

		auto& camera = component.camera;

		out << YAML::Key << "Camera" << YAML::Value;
		out << YAML::BeginMap; // Camera
		out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
		out << YAML::Key << "PerspectiveFoV" << YAML::Value << camera.GetPerspectiveFoV();
		out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
		out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
		out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
		out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
		out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
		out << YAML::EndMap; // Camera

		out << YAML::Key << "Primary" << YAML::Value << component.primary;
		out << YAML::Key << "FixedAspectRatio" << YAML::Value << component.fixedAspectRatio;

		out << YAML::EndMap; // CameraComponent
	}

	template<>
	static void SerializeComponent<SpriteComponent>(YAML::Emitter& out, Entity entity, const SpriteComponent& component)
	{
		out << YAML::Key << "SpriteComponent";
		out << YAML::BeginMap; // SpriteComponent

		out << YAML::Key << "Color" << YAML::Value << component.color;
		if (component.texture)
			out << YAML::Key << "TexturePath" << YAML::Value << component.texture->GetPath();
		out << YAML::Key << "TilingFactor" << YAML::Value << component.tilingFactor;

		out << YAML::EndMap; // SpriteComponent
	}

	template<>
	static void SerializeComponent<CircleComponent>(YAML::Emitter& out, Entity entity, const CircleComponent& component)
	{
		out << YAML::Key << "CircleComponent";
		out << YAML::BeginMap; // CircleComponent

		out << YAML::Key << "Color" << YAML::Value << component.color;
		out << YAML::Key << "Thickness" << YAML::Value << component.thickness;
		out << YAML::Key << "Fade" << YAML::Value << component.fade;

		out << YAML::EndMap; // CircleComponent
	}

	template<>
	static void SerializeComponent<RigidBody2DComponent>(YAML::Emitter& out, Entity entity, const RigidBody2DComponent& component)
	{
		out << YAML::Key << "RigidBody2DComponent";
		out << YAML::BeginMap; // RigidBody2DComponent

		out << YAML::Key << "BodyType" << YAML::Value << BodyTypeToString(component.bodyType);
		out << YAML::Key << "FixedRotation" << YAML::Value << component.fixedRotation;

		out << YAML::EndMap; // RigidBody2DComponent
	}

	template<>
	static void SerializeComponent<BoxCollider2DComponent>(YAML::Emitter& out, Entity entity, const BoxCollider2DComponent& component)
	{
		out << YAML::Key << "BoxCollider2DComponent";
		out << YAML::BeginMap; // BoxCollider2DComponent

		out << YAML::Key << "Offset" << YAML::Value << component.offset;
		out << YAML::Key << "Size" << YAML::Value << component.size;
		out << YAML::Key << "Density" << YAML::Value << component.density;
		out << YAML::Key << "Friction" << YAML::Value << component.friction;
		out << YAML::Key << "Restitution" << YAML::Value << component.restitution;
		out << YAML::Key << "RestitutionThreshold" << YAML::Value << component.restitutionThreshold;

		out << YAML::EndMap; // BoxCollider2DComponent
	}

	template<>
	static void SerializeComponent<CircleCollider2DComponent>(YAML::Emitter& out, Entity entity, const CircleCollider2DComponent& component)
	{
		out << YAML::Key << "CircleCollider2DComponent";
		out << YAML::BeginMap; // CircleCollider2DComponent

		out << YAML::Key << "Offset" << YAML::Value << component.offset;
		out << YAML::Key << "Radius" << YAML::Value << component.radius;
		out << YAML::Key << "Density" << YAML::Value << component.density;
		out << YAML::Key << "Friction" << YAML::Value << component.friction;
		out << YAML::Key << "Restitution" << YAML::Value << component.restitution;
		out << YAML::Key << "RestitutionThreshold" << YAML::Value << component.restitutionThreshold;

		out << YAML::EndMap; // CircleCollider2DComponent
	}




	// Deserialize components
	template<typename... T>
	static void DeserializeAllComponents(YAML::detail::iterator_value entity, Entity deserializedEntity)
	{
		([&]()
			{
				T temp{};
				auto node = entity[temp.name];
				if (node)
				{
					if (deserializedEntity.HasComponent<T>())
					{
						auto& comp = deserializedEntity.GetComponent<T>();
						RB_CORE_INFO("Deserializing {} for entity {}", comp.name, deserializedEntity.GetUUID());
						DeserializeComponent<T>(node, comp);
					}
					else
					{
						auto& comp = deserializedEntity.AddComponent<T>();
						DeserializeComponent<T>(node, comp);
					}
				}

			}(), ...);

	}

	template<typename... T>
	static void DeserializeAllComponents(ComponentGroup<T...>, YAML::detail::iterator_value entity, Entity deserializedEntity)
	{
		DeserializeAllComponents<T...>(entity, deserializedEntity);
	}

	// Not sure I like this design. Might be better to just go back to if statements for components, but this makes it easier to navigate and find them via VA Outline
	template<typename C>
	static void DeserializeComponent(YAML::Node node, C& component) {}

	template<>
	static void DeserializeComponent<TransformComponent>(YAML::Node node, TransformComponent& component)
	{
		component.translation = node["Translation"].as<glm::vec3>();
		component.rotation = node["Rotation"].as<glm::vec3>();
		component.scale = node["Scale"].as<glm::vec3>();
	}

	template<>
	static void DeserializeComponent<CameraComponent>(YAML::Node node, CameraComponent& component)
	{
		auto& cameraProps = node["Camera"];
		component.camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());
		component.camera.SetPerspectiveFoV(cameraProps["PerspectiveFoV"].as<float>());
		component.camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
		component.camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());
		component.camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
		component.camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
		component.camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());
		component.primary = node["Primary"].as<bool>();
		component.fixedAspectRatio = node["FixedAspectRatio"].as<bool>();
	}

	template<>
	static void DeserializeComponent<SpriteComponent>(YAML::Node node, SpriteComponent& component)
	{
		component.color = node["Color"].as<glm::vec4>();
		if (node["TexturePath"])
			component.texture = Texture2D::Create(node["TexturePath"].as<std::string>());
		if (node["TilingFactor"])
			component.tilingFactor = node["TilingFactor"].as<float>();
	}

	template<>
	static void DeserializeComponent<CircleComponent>(YAML::Node node, CircleComponent& component)
	{
		component.color = node["Color"].as<glm::vec4>();
		component.thickness = node["Thickness"].as<float>();
		component.fade = node["Fade"].as<float>();
	}

	template<>
	static void DeserializeComponent<RigidBody2DComponent>(YAML::Node node, RigidBody2DComponent& component)
	{
		component.bodyType = BodyTypeFromString(node["BodyType"].as<std::string>());
		component.fixedRotation = node["FixedRotation"].as<bool>();
	}

	template<>
	static void DeserializeComponent<BoxCollider2DComponent>(YAML::Node node, BoxCollider2DComponent& component)
	{
		component.offset = node["Offset"].as<glm::vec2>();
		component.size = node["Size"].as<glm::vec2>();
		component.density = node["Density"].as<float>();
		component.friction = node["Friction"].as<float>();
		component.restitution = node["Restitution"].as<float>();
		component.restitutionThreshold = node["RestitutionThreshold"].as<float>();
	}

	template<>
	static void DeserializeComponent<CircleCollider2DComponent>(YAML::Node node, CircleCollider2DComponent& component)
	{
		component.offset = node["Offset"].as<glm::vec2>();
		component.radius = node["Radius"].as<float>();
		component.density = node["Density"].as<float>();
		component.friction = node["Friction"].as<float>();
		component.restitution = node["Restitution"].as<float>();
		component.restitutionThreshold = node["RestitutionThreshold"].as<float>();
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : mScene(scene)
	{

	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		RB_CORE_ASSERT(entity.HasComponent<IDComponent>(), "Entities must have a UUID IDComponent");
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();
		SerializeAllComponents(AllComponents{}, out, entity);
		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::SerializeToYaml(const std::string& filepath)
	{
		RB_PROFILE_FUNC();
		RB_CORE_TRACE("Serializing scene to {}", filepath);
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		mScene->mRegistry.each([&](auto entId)
			{
				Entity entity = { entId, mScene.get() };
				if (!entity) return;
				SerializeEntity(out, entity);
			});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeToBinary(const std::string& filepath)
	{
		throw std::logic_error("Method not yet implemented");
	}

	bool SceneSerializer::DeserializeFromYaml(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data;
		try {
			data = YAML::Load(strStream.str());
		}
		catch (YAML::ParserException e)
		{
			RB_CORE_ERROR("Failed to load scene file {}", filepath);
			return false;
		}

		if (!data["Scene"])
		{
			RB_CORE_ERROR("Scene file {} does not contain Scene node", filepath);
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();
		RB_CORE_INFO("Deserializing scene '{}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64 uuid = entity["Entity"].as<uint64>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				Entity deserializedEntity = mScene->CreateEntityWithUUID(uuid, name);

				DeserializeAllComponents(AllComponents_NoID_NoTag{}, entity, deserializedEntity);
				RB_CORE_INFO("Deserialized entity with ID = {}, name = {}", uuid, name);
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeFromBinary(const std::string& filepath)
	{
		RB_CORE_ASSERT(false, "Not yet implemented");
		return false;
	}

}