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
#include "rebirth/renderer/Renderer2D.h"
#include "Entity.h"

namespace rebirth
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		Entity entity = { mRegistry.create(), this };
		auto& t = entity.AddComponent<TagComponent>();
		t.tag = tag.empty() ? "Entity" : tag;

		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = mRegistry.group<TransformComponent>(entt::get<SpriteComponent>);
		for (auto entity : group)
		{
			auto& [trans, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
			Renderer2D::DrawQuad(trans, sprite.color);
		}
	}

}

