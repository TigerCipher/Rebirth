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
			default: break;
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

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : mScene(scene)
	{

	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		RB_CORE_ASSERT(entity.HasComponent<IDComponent>(), "Entities must have a UUID IDComponent");
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tag = entity.GetComponent<TagComponent>().tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // TagComponent
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.scale;

			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.camera;

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

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.fixedAspectRatio;

			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<SpriteComponent>())
		{
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap; // SpriteComponent

			auto& spriteComp = entity.GetComponent<SpriteComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteComp.color;
			if (spriteComp.texture)
				out << YAML::Key << "TexturePath" << YAML::Value << spriteComp.texture->GetPath();
			out << YAML::Key << "TilingFactor" << YAML::Value << spriteComp.tilingFactor;

			out << YAML::EndMap; // SpriteComponent
		}

		if (entity.HasComponent<CircleComponent>())
		{
			out << YAML::Key << "CircleComponent";
			out << YAML::BeginMap; // CircleComponent

			auto& spriteComp = entity.GetComponent<CircleComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteComp.color;
			out << YAML::Key << "Thickness" << YAML::Value << spriteComp.thickness;
			out << YAML::Key << "Fade" << YAML::Value << spriteComp.fade;

			out << YAML::EndMap; // CircleComponent
		}

		if (entity.HasComponent<RigidBody2DComponent>())
		{
			out << YAML::Key << "RigidBody2DComponent";
			out << YAML::BeginMap; // RigidBody2DComponent

			auto& rbComp = entity.GetComponent<RigidBody2DComponent>();
			out << YAML::Key << "BodyType" << YAML::Value << BodyTypeToString(rbComp.bodyType);
			out << YAML::Key << "FixedRotation" << YAML::Value << rbComp.fixedRotation;

			out << YAML::EndMap; // RigidBody2DComponent
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap; // BoxCollider2DComponent

			auto& boxComp = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << boxComp.offset;
			out << YAML::Key << "Size" << YAML::Value << boxComp.size;
			out << YAML::Key << "Density" << YAML::Value << boxComp.density;
			out << YAML::Key << "Friction" << YAML::Value << boxComp.friction;
			out << YAML::Key << "Restitution" << YAML::Value << boxComp.restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << boxComp.restitutionThreshold;

			out << YAML::EndMap; // BoxCollider2DComponent
		}

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
		RB_CORE_ASSERT(false, "Not yet implemented");
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
			return false;
		}

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		RB_CORE_TRACE("Deserializing scene '{}'", sceneName);

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

				RB_CORE_TRACE("Deserialized entity with ID = {}, name = {}", uuid, name);

				Entity deserializedEntity = mScene->CreateEntityWithUUID(uuid, name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.translation = transformComponent["Translation"].as<glm::vec3>();
					tc.rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];
					cc.camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.camera.SetPerspectiveFoV(cameraProps["PerspectiveFoV"].as<float>());
					cc.camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.primary = cameraComponent["Primary"].as<bool>();
					cc.fixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteComponent>();
					src.color = spriteRendererComponent["Color"].as<glm::vec4>();
					if (spriteRendererComponent["TexturePath"])
						src.texture = Texture2D::Create(spriteRendererComponent["TexturePath"].as<std::string>());
					if (spriteRendererComponent["TilingFactor"])
						src.tilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
				}

				auto circleComp = entity["CircleComponent"];
				if (circleComp)
				{
					auto& src = deserializedEntity.AddComponent<CircleComponent>();
					src.color = circleComp["Color"].as<glm::vec4>();
					src.thickness = circleComp["Thickness"].as<float>();
					src.fade = circleComp["Fade"].as<float>();
				}

				auto rigidComp = entity["RigidBody2DComponent"];
				if (rigidComp)
				{
					auto& rb = deserializedEntity.AddComponent<RigidBody2DComponent>();
					rb.bodyType = BodyTypeFromString(rigidComp["BodyType"].as<std::string>());
					rb.fixedRotation = rigidComp["FixedRotation"].as<bool>();
				}

				auto boxComp = entity["BoxCollider2DComponent"];
				if (rigidComp)
				{
					auto& bc = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					bc.offset = boxComp["Offset"].as<glm::vec2>();
					bc.size = boxComp["Size"].as<glm::vec2>();
					bc.density = boxComp["Density"].as<float>();
					bc.friction = boxComp["Friction"].as<float>();
					bc.restitution = boxComp["Restitution"].as<float>();
					bc.restitutionThreshold = boxComp["RestitutionThreshold"].as<float>();
				}
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