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
// File Name: Reference.h
// Date File Created: 7/27/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "rebirth/core/Common.h"
#include "rebirth/debug/Log.h"

namespace rebirth
{
	class RefCount
	{
	public:

		RefCount() = default;
		virtual ~RefCount() = default;

		void IncRefCount() const { ++mRefCount; }
		void DecRefCount() const { --mRefCount; }

		uint32 GetRefCount() const { return mRefCount; }

	private:
		mutable uint32 mRefCount = 0;
	};

	// TODO This class will be hopefully a higher performance lighter weight but still thread safe shared pointer variant
	template<typename T>
	class Reference
	{
	public:
		Reference() = default;
		Reference(std::nullptr_t) : mInstance(nullptr) {}
		Reference(T* instance) : mInstance(instance)
		{
			static_assert(std::is_base_of<RefCount, T>::value, "Class must be a child of RefCount");
			IncRef();
		}
		Reference(const Reference<T>& other) : mInstance(other.mInstance)
		{
			IncRef();
		}
		template<typename T2>
		Reference(const Reference<T2>& other)
		{
			mInstance = (T*)other.mInstance;
			IncRef();
		}
		template<typename T2>
		Reference(Reference<T2>&& other)
		{
			mInstance = (T*)other.mInstance;
			other.mInstance = nullptr;
		}

		~Reference()
		{
			DecRef();
		}

		Reference& operator=(std::nullptr_t)
		{
			DecRef();
			mInstance = nullptr;
			return *this;
		}

		Reference& operator=(const Reference<T>& other)
		{
			other.IncRef();
			DecRef();
			mInstance = other.mInstance;
			return *this;
		}

		template<typename T2>
		Reference& operator=(const Reference<T2>& other)
		{
			other.IncRef();
			DecRef();
			mInstance = other.mInstance;
			return *this;
		}

		template<typename T2>
		Reference& operator=(Reference<T2>&& other)
		{
			DecRef();
			mInstance = other.mInstance;
			other.mInstance = nullptr;
			return *this;
		}

		T* operator->() { return mInstance; }
		const T* operator->() const { return mInstance; }

		T& operator*() { return *mInstance; }
		const T& operator*() const { return *mInstance; }

		bool operator==(const Reference<T>& other) const { return mInstance == other.mInstance; }

		operator bool() { return mInstance != nullptr; }
		operator bool() const { return mInstance != nullptr; }

		template<typename T2>
		Reference<T2> As() const { return Reference<T2>(*this); }

		T* Raw() { return mInstance; }
		const T* Raw() const { return mInstance; }

		void Release(T* instance = nullptr)
		{
			DecRef();
			mInstance = instance;
		}

		template<typename... Args>
		static Reference<T> Create(Args&&... args)
		{
			return Reference<T>(new T(std::forward<Args>(args)...));
		}
	private:

		void IncRef() const
		{
			if (mInstance)
				mInstance->IncRefCount();
		}

		void DecRef() const
		{
			if (mInstance)
			{
				mInstance->DecRefCount();
				if (mInstance->GetRefCount() == 0)
					delete mInstance;
			}
		}

	private:
		template<class T2>
		friend class Reference;

		T* mInstance = nullptr;
	};


	template <typename T, typename... Args>
	constexpr Reference<T> createReference(Args&& ...args)
	{
		return Reference<T>::Create(std::forward<Args>(args)...);
	}
}