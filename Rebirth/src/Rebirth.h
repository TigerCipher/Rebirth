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
// File Name: Rebirth.h
// Date File Created: 06/13/2022 at 3:12 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once

// Core
#include "rebirth/core/Application.h"
#include "rebirth/core/Layer.h"
#include "rebirth/core/Timestep.h"

// Debug
#include "rebirth/debug/Log.h"

// Events
#include "rebirth/events/KeyEvent.h"
#include "rebirth/events/MouseEvent.h"


// Input
#include "rebirth/input/Input.h"
#include "rebirth/input/KeyCodes.h"
#include "rebirth/input/MouseCodes.h"


// Renderer
#include "rebirth/renderer/Renderer.h"
#include "rebirth/renderer/RenderCommand.h"
#include "rebirth/renderer/Shader.h"
#include "rebirth/renderer/VertexArray.h"
#include "rebirth/renderer/OrthoCamera.h"

// GUI
#include "rebirth/imgui/ImguiLayer.h"


// Main function
#include "rebirth/core/EntryPoint.h"