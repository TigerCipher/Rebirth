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
// File Name: OrthoCamera.h
// Date File Created: 6/21/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once




namespace rebirth
{
	class OrthoCamera
	{
	public:
		OrthoCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		void SetPosition(const glm::vec3& pos);
		const glm::vec3& GetPosition() const { return mPosition; }

		void SetRotation(float rot);
		float GetRotation() { return mRotation; }

		const glm::mat4& ProjectionMatrix() const { return mProjection; }
		const glm::mat4& ViewMatrix() const { return mView; }

		
		//************************************
		// Important note: In OpenGL, this is a Projection View matrix (proj x view),
		// currently view x proj matrices are not implemented
		//************************************
		const glm::mat4& ViewProjectionMatrix() const { return mViewProjection; }

	private:

		void RecalculateViewMatrix();

		glm::mat4 mProjection{};
		glm::mat4 mView{};
		glm::mat4 mViewProjection{};

		glm::vec3 mPosition{0.0f};
		float mRotation = 0;
	};
}

