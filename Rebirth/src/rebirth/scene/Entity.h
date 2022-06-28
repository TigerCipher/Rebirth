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
// File Name: Entity.h
// Date File Created: 6/27/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "Scene.h"
#include <entt.hpp>

namespace rebirth
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity id, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			RB_CORE_ASSERT(!HasComponent<T>(), "Component already added to this entity");
			return mScene->mRegistry.emplace<T>(mId, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			RB_CORE_ASSERT(HasComponent<T>(), "This entity does not have this component");
			return mScene->mRegistry.get<T>(mId);
		}

		template<typename T>
		bool HasComponent()
		{
			if(mId != entt::null) return mScene->mRegistry.any_of<T>(mId);
			return false;
		}

		template<typename T>
		void RemoveComponent()
		{
			RB_CORE_ASSERT(HasComponent<T>(), "This entity does not have this component");
			mScene->mRegistry.remove<T>(mId);
		}

		operator bool() const { return mId != entt::null; }
		operator uint32() const { return (uint32)mId; }

		bool operator==(const Entity& other)
		{
			return mId == other.mId && mScene == other.mScene;
		}

		bool operator!=(const Entity& other)
		{
			return !(*this == other);
		}
	private:
		entt::entity mId = entt::null;
		Scene* mScene = nullptr;
	};
}


