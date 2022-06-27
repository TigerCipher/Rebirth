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
// File Name: OrthoCameraController.h
// Date File Created: 6/23/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "rebirth/renderer/OrthoCamera.h"
#include "rebirth/core/Timestep.h"
#include "rebirth/events/AppEvent.h"
#include "rebirth/events/MouseEvent.h"

namespace rebirth
{
	struct OrthoCameraBounds
	{
		float left, right;
		float bottom, top;

		float GetWidth() { return right - left; }
		float GetHeight() { return top - bottom; }
	};

	class OrthoCameraController
	{
	public:
		OrthoCameraController(const float aspectRatio, const bool useRotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void ResizeBounds(float width, float height);

		void SetZoom(const float zoom) { mZoom = zoom; CalculateView(); }
		float GetZoom() const { return mZoom; }

		OrthoCamera& GetCamera() { return mCamera; }
		const OrthoCamera& GetCamera() const { return mCamera; }

		const OrthoCameraBounds& GetBounds() const { return mBounds; }
	private:
		void CalculateView();
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);


		float mAspectRatio;
		bool mUseRotation;

		float mZoom = 1.0f;
		OrthoCameraBounds mBounds;
		OrthoCamera mCamera;

		glm::vec3 mCamPos{ 0.0f };
		float mCamRotation = 0.0f;
		
		float mCamSpeed = 2.0f;
		float mCamRotationSpeed = 60.0f;
	};
}

