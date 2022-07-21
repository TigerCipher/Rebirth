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
// File Name: SceneEvent.h
// Date File Created: 7/19/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "Event.h"
#include "rebirth/scene/Scene.h"
#include "rebirth/scene/Entity.h"


namespace rebirth
{
	class SceneEvent : public Event
	{
	public:
		const Ref<Scene>& GetScene() const { return mScene; }
		Ref<Scene> GetScene() { return mScene; }

		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP | EVENT_CATEGORY_SCENE)
	protected:
		SceneEvent(EventType type, const Ref<Scene>& scene) : Event(type), mScene(scene) {}
		Ref<Scene> mScene;
	};

	class ScenePreStartEvent : public SceneEvent
	{
	public:
		ScenePreStartEvent(const Ref<Scene>& scene)
			: SceneEvent(EventType::SCENE_PRE_START, scene) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ScenePreStartEvent: " << "UNTITLED TEMP";//mScene->GetName();
			return ss.str();
		}

		//EVENT_CLASS_TYPE(SCENE_PRE_START)
	};

	class ScenePostStartEvent : public SceneEvent
	{
	public:
		ScenePostStartEvent(const Ref<Scene>& scene)
			: SceneEvent(EventType::SCENE_POST_START, scene) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ScenePostStartEvent: " << "UNTITLED TEMP";//mScene->GetName();
			return ss.str();
		}

		//EVENT_CLASS_TYPE(SCENE_POST_START)
	};

	class ScenePreStopEvent : public SceneEvent
	{
	public:
		ScenePreStopEvent(const Ref<Scene>& scene)
			: SceneEvent(EventType::SCENE_PRE_STOP, scene) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ScenePreStopEvent: " << "UNTITLED TEMP";//mScene->GetName();
			return ss.str();
		}

		//EVENT_CLASS_TYPE(SCENE_PRE_STOP)
	};

	class ScenePostStopEvent : public SceneEvent
	{
	public:
		ScenePostStopEvent(const Ref<Scene>& scene)
			: SceneEvent(EventType::SCENE_POST_STOP, scene) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "ScenePostStopEvent: " << "UNTITLED TEMP";//mScene->GetName();
			return ss.str();
		}

		//EVENT_CLASS_TYPE(SCENE_POST_STOP)
	};
}

