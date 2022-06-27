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
// File Name: Scene.h
// Date File Created: 6/27/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include <entt.hpp>

#include "rebirth/core/Timestep.h"

namespace rebirth
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		Entity CreateEntity(const std::string& tag = std::string());

		void OnUpdate(Timestep ts);

	private:
		entt::registry mRegistry;

		friend class Entity;
	};
}
