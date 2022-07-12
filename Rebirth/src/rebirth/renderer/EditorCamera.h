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
// File Name: EditorCamera.h
// Date File Created: 7/11/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once
#include "Camera.h"
#include "rebirth/core/Timestep.h"
#include "rebirth/events/Event.h"
#include "rebirth/events/MouseEvent.h"

namespace rebirth
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void SetDistance(float distance) { mDistance = distance; }
		float GetDistance() const { return mDistance; }

		void SetViewportSize(float width, float height) { mViewportWidth = width; mViewportHeight = height; UpdateProjection(); }

		const glm::mat4& GetViewMatrix() const { return mViewMatrix; }
		glm::mat4 GetViewProjection() const { return mProjection * mViewMatrix; }

		glm::vec3 GetUp() const;
		glm::vec3 GetRight() const;
		glm::vec3 GetForward() const;

		const glm::vec3& GetPosition() const { return mPosition; }

		glm::quat GetOrientation() const;

		float GetPitch() const { return mPitch; }
		float GetYaw() const { return mYaw; }

	private:

		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);
		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;



		float mFov = 45.0f;
		float mAspectRatio = 1.778f;
		float mNear = 0.1f;
		float mFar = 1000.0f;

		glm::mat4 mViewMatrix;
		glm::vec3 mPosition{ 0.0f };
		glm::vec3 mFocal{ 0.0f };

		glm::vec2 mInitialMousePos{ 0.0f };

		float mDistance = 10.0f;
		float mPitch = 0.0f;
		float mYaw = 0.0f;

		float mViewportWidth = 1280.0f;
		float mViewportHeight = 720.0f;
	};
}

