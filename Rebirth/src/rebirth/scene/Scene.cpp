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
// File Name: Scene.cpp
// Date File Created: 6/27/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "Scene.h"

#include "Components.h"
#include "ScriptableEntity.h"
#include "rebirth/renderer/Renderer2D.h"
#include "Entity.h"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

namespace rebirth
{

	static b2BodyType GetBodyType(RigidBody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case RigidBody2DComponent::BodyType::STATIC: return b2_staticBody;
			case RigidBody2DComponent::BodyType::DYNAMIC: return b2_dynamicBody;
			case RigidBody2DComponent::BodyType::KINEMATIC: return b2_kinematicBody;
			default: break;
		}

		RB_CORE_ASSERT(false, "Unknown physics body type");
		return b2_staticBody;
	}

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	template<typename T>
	static void CopyComponent(const entt::registry& src, entt::registry& dest, std::unordered_map<UUID, entt::entity> enttMap)
	{
		auto view = src.view<T>();

		for (auto e : view)
		{
			UUID uuid = src.get<IDComponent>(e).uuid;
			RB_CORE_ASSERT(enttMap.find(uuid) != enttMap.end());
			entt::entity enttId = enttMap.at(uuid);
			auto& comp = src.get<T>(e);
			dest.emplace_or_replace<T>(enttId, comp);
		}
	}

	template<typename T>
	static void CopyComponentIfExists(Entity src, Entity dest)
	{
		if (src.HasComponent<T>())
		{
			dest.AddOrReplaceComponent<T>(src.GetComponent<T>());
		}
	}

	Ref<Scene> Scene::Copy(Ref<Scene> src)
	{
		Ref<Scene> newScene = createRef<Scene>();

		newScene->mViewportWidth = src->mViewportWidth;
		newScene->mViewportHeight = src->mViewportHeight;

		std::unordered_map<UUID, entt::entity> enttMap;

		auto& srcReg = src->mRegistry;
		auto& destReg = newScene->mRegistry;
		auto idView = srcReg.view<IDComponent>();

		for (auto e : idView)
		{
			UUID uuid = srcReg.get<IDComponent>(e).uuid;
			const auto& tag = srcReg.get<TagComponent>(e).tag;
			enttMap[uuid] = newScene->CreateEntityWithUUID(uuid, tag);
		}

		CopyComponent<TransformComponent>(srcReg, destReg, enttMap);
		CopyComponent<SpriteComponent>(srcReg, destReg, enttMap);
		CopyComponent<CameraComponent>(srcReg, destReg, enttMap);
		CopyComponent<NativeScriptComponent>(srcReg, destReg, enttMap);
		CopyComponent<RigidBody2DComponent>(srcReg, destReg, enttMap);
		CopyComponent<BoxCollider2DComponent>(srcReg, destReg, enttMap);

		return newScene;
	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		return CreateEntityWithUUID(UUID(), tag);
	}

	rebirth::Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& tag /*= std::string()*/)
	{
		Entity entity = { mRegistry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		auto& t = entity.AddComponent<TagComponent>();
		t.tag = tag.empty() ? "Entity" : tag;

		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void Scene::DuplicateEntity(Entity entity)
	{
		Entity newEnt = CreateEntity(entity.GetTag());

		CopyComponentIfExists<TransformComponent>(entity, newEnt);
		CopyComponentIfExists<SpriteComponent>(entity, newEnt);
		CopyComponentIfExists<CameraComponent>(entity, newEnt);
		CopyComponentIfExists<NativeScriptComponent>(entity, newEnt);
		CopyComponentIfExists<RigidBody2DComponent>(entity, newEnt);
		CopyComponentIfExists<BoxCollider2DComponent>(entity, newEnt);
	}

	void Scene::DestroyEntity(Entity entity)
	{
		mRegistry.destroy(entity);
	}

	void Scene::OnRuntimeStart()
	{
		mPhysicsWorld = new b2World({ 0.0f, -9.8f });
		auto view = mRegistry.view<RigidBody2DComponent>();
		for (auto e : view)
		{
			Entity ent = { e, this };
			auto& transform = ent.GetComponent<TransformComponent>();
			auto& rb = ent.GetComponent<RigidBody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = GetBodyType(rb.bodyType);
			bodyDef.position.Set(transform.translation.x, transform.translation.y);
			bodyDef.angle = transform.rotation.z;

			b2Body* body = mPhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb.fixedRotation);
			rb.runtimeBody = body;

			if (ent.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc = ent.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(transform.scale.x * bc.size.x, transform.scale.y * bc.size.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc.density;
				fixtureDef.friction = bc.friction;
				fixtureDef.restitution = bc.restitution;
				fixtureDef.restitutionThreshold = bc.restitutionThreshold;

				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		RB_DELETE(mPhysicsWorld);
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		RB_PROFILE_FUNC();

		// Scripting
		{
			mRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					if (!nsc.instance)
					{
						nsc.instance = nsc.InstantiateScript();
						nsc.instance->mEntity = Entity{ entity, this };
						nsc.instance->OnCreate();
					}

					nsc.instance->OnUpdate(ts);
				});
		}

		// Physics
		{
			const int32 velocityIterations = 6;
			const int32 positionIterations = 2;
			mPhysicsWorld->Step(ts, velocityIterations, positionIterations);

			auto view = mRegistry.view<RigidBody2DComponent>();

			for (auto e : view)
			{
				Entity ent = { e, this };
				auto& transform = ent.GetComponent<TransformComponent>();
				auto& rb = ent.GetComponent<RigidBody2DComponent>();

				b2Body* body = (b2Body*)rb.runtimeBody;

				const auto& position = body->GetPosition();
				transform.translation.x = position.x;
				transform.translation.y = position.y;
				transform.rotation.z = body->GetAngle();
			}
		}


		// Render
		Camera* camera = nullptr;
		glm::mat4 transform;
		{
			auto view = mRegistry.view<TransformComponent, CameraComponent>();

			for (auto entity : view)
			{
				auto [trans, cam] = view.get<TransformComponent, CameraComponent>(entity);
				if (cam.primary)
				{
					camera = &cam.camera;
					transform = trans.GetTransform();
					break;
				}

			}
		}

		if (camera)
		{
			Renderer2D::BeginScene(camera->GetProjection(), transform);
			auto group = mRegistry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto entity : group)
			{
				auto [trans, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
				Renderer2D::DrawSprite(trans.GetTransform(), sprite, (int)entity);
			}

			Renderer2D::EndScene();
		}

	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		auto group = mRegistry.group<TransformComponent>(entt::get<SpriteComponent>);
		for (auto entity : group)
		{
			auto [trans, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
			Renderer2D::DrawSprite(trans.GetTransform(), sprite, (int)entity);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(uint32 width, uint32 height)
	{
		mViewportWidth = width;
		mViewportHeight = height;

		auto view = mRegistry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComp = view.get<CameraComponent>(entity);
			if (!cameraComp.fixedAspectRatio)
			{
				cameraComp.camera.SetViewportSize(width, height);
			}
		}
	}


	void Scene::DestroyAll()
	{
		mRegistry.clear();
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = mRegistry.view<CameraComponent>();
		for (auto e : view)
		{
			const auto& cam = view.get<CameraComponent>(e);
			if (cam.primary)
				return Entity{ e, this };
		}

		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<SpriteComponent>(Entity entity, SpriteComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if(mViewportWidth > 0 && mViewportHeight > 0)
			component.camera.SetViewportSize(mViewportWidth, mViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{

	}



}

