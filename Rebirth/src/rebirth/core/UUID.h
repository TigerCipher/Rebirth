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
// File Name: UUID.h
// Date File Created: 7/15/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include <xhash>

namespace rebirth
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64 id);
		UUID(const UUID&) = default;

		operator uint64() const { return mUUID; }
	private:
		uint64 mUUID;
	};
}


namespace std
{
	template<>
	struct hash<rebirth::UUID>
	{
		std::size_t operator()(const rebirth::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}

