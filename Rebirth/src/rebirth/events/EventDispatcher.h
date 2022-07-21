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
// File Name: EventDispatcher.h
// Date File Created: 7/20/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "Event.h"
#include "EventListener.h"

namespace rebirth
{

	class EventDispatcher
	{
	public:
		EventDispatcher(const std::string& name) : mName(name) {}
		virtual ~EventDispatcher() = default;

		void PollEvents();

		void Dispatch(Event* evt);

		void AddListener(EventListener* listener);
		void RemoveListener(EventListener* listener);

	private:
		std::string mName;
		std::vector<EventListener*> mListeners;
		std::queue<Event*> mEvents;
	};

	//class EventDispatcher
	//{
	//public:
	//	EventDispatcher(const std::string& name, uint32 threadCount = 1);
	//	virtual ~EventDispatcher();

	//	void Dispatch(Event& evt);

	//	void AddListener(const Ref<EventListener>& listener);
	//	void RemoveListener(const Ref<EventListener>& listener);

	//private:
	//	void DispatchThreadHandler();

	//	std::string mName;
	//	std::mutex mLock;
	//	std::vector<std::thread> mThreads;
	//	std::vector<Ref<EventListener>> mListeners;
	//	std::queue<Ref<Event>> mQueue;
	//	std::condition_variable mCondition;
	//	bool mQuit = false;
	//};
}

