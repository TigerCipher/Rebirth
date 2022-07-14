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
// File Name: Texture.h
// Date File Created: 6/22/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once



namespace rebirth
{
	class Texture
	{
	public:
		virtual void Bind(uint32 slot = 0) const = 0;

		virtual void SetData(void* data, uint32 size) = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual uint32 GetId() const = 0;

		virtual bool IsLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(uint32 width, uint32 height);

	};
}