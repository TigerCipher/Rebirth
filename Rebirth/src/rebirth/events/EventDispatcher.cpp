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
// File Name: EventDispatcher.cpp
// Date File Created: 7/20/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "EventDispatcher.h"
#include "rebirth/util/PlatformUtil.h"

namespace rebirth
{
		//		if (!mQueue.empty())
	//		{
	//			auto& evt = mQueue.front();
	//			mQueue.pop();
	//			lock.unlock();
	//			evt->Invoke();

	//			for (const auto& listener : mListeners)
	//			{
	//				listener->OnEvent(evt);
	//			}
	//			evt->handled = true;
	//			lock.lock();
	//		}

	void EventDispatcher::PollEvents()
	{
		RB_PROFILE_FUNC();
		if (!mEvents.empty())
		{
			auto& evt = mEvents.front();
			mEvents.pop();
			evt->Invoke();

			for (const auto& listener : mListeners)
			{
				if(!evt->handled)
					listener->OnEvent(*evt);
			}

			if (evt) delete evt;
		}
	}

	void EventDispatcher::Dispatch(Event* evt)
	{
		mEvents.push(evt);
	}

	void EventDispatcher::AddListener(EventListener* listener)
	{
		mListeners.push_back(listener);
	}

	void EventDispatcher::RemoveListener(EventListener* listener)
	{
		std::vector<EventListener*>::iterator it;
		for (auto it = mListeners.begin(); it != mListeners.end(); it++)
		{
			if ((*it) == listener)
				it = mListeners.erase(it);
		}
	}


	//EventDispatcher::EventDispatcher(const std::string& name, uint32 threadCount /*= 1*/) :
	//	mName(name),
	//	mThreads(threadCount)
	//{
	//	for (auto& thread : mThreads)
	//	{
	//		thread = std::thread(&EventDispatcher::DispatchThreadHandler, this);
	//	}
	//}

	//EventDispatcher::~EventDispatcher()
	//{
	//	std::unique_lock<std::mutex> lock(mLock);
	//	mQuit = true;
	//	lock.unlock();
	//	mCondition.notify_all();

	//	for (auto& t : mThreads)
	//	{
	//		if (t.joinable())
	//			t.join();
	//	}
	//}

	//void EventDispatcher::Dispatch(Event& evt)
	//{
	//	std::unique_lock<std::mutex> lock(mLock);
	//	mQueue.push(evt);
	//	lock.unlock();
	//	mCondition.notify_one();
	//}

	//void EventDispatcher::AddListener(const Ref<EventListener>& listener)
	//{
	//	mListeners.push_back(listener);
	//}

	//void EventDispatcher::RemoveListener(const Ref<EventListener>& listener)
	//{
	//	std::vector<Ref<EventListener>>::iterator it;
	//	for (auto it = mListeners.begin(); it != mListeners.end(); it++)
	//	{
	//		if ((*it) == listener)
	//			it = mListeners.erase(it);
	//	}
	//}

	//void EventDispatcher::DispatchThreadHandler()
	//{
	//	std::unique_lock<std::mutex> lock(mLock);

	//	do
	//	{
	//		mCondition.wait(lock, [this]
	//			{
	//				return !mQueue.empty() || mQuit;
	//			});

	//		if (!mQueue.empty())
	//		{
	//			auto& evt = mQueue.front();
	//			mQueue.pop();
	//			lock.unlock();
	//			evt->Invoke();

	//			for (const auto& listener : mListeners)
	//			{
	//				listener->OnEvent(evt);
	//			}
	//			evt->handled = true;
	//			lock.lock();
	//		}

	//	} while (!mQuit);
	//}



}

