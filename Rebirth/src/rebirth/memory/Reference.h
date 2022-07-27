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

		T* operator->() { return mInstance; }
		const T* operator->() const { return mInstance; }

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
}