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
// File Name: Sandbox2D.h
// Date File Created: 6/24/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include <Rebirth.h>

class Sandbox2D : public rebirth::Layer
{
public:
	Sandbox2D() : Layer("Sandbox2D"), mCameraController(1920.0f / 1080.0f, true) {}
	virtual ~Sandbox2D() = default;
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(rebirth::Timestep ts) override;
	void OnEvent(rebirth::Event& e) override;
	void OnImguiRender() override;
private:
	rebirth::OrthoCameraController mCameraController;

	// abstract to renderer
	Ref<rebirth::Shader> mShader;
	Ref<rebirth::VertexArray> mSquareVtxArray;
	glm::vec4 mSquareColor{ 0.6f, 0.2f, 0.2f, 1.0f };
	glm::vec2 mSquarePos{ 0.0f, 0.0f};
	glm::vec2 mSquareSize{ 0.5f, 0.64f};
	float mSquareAngle = 45.0f;

	Ref<rebirth::Texture2D> mTexture;
};