#pragma once

//For client usage
#include <string>
#include <memory>
#include "Violet/Core/Core.h"
#include "Violet/Core/Log.h"
#include "Violet/Core/Application.h"
#include "Violet/ImGui/ImGuiLayer.h"
#include "Violet/Core/Input.h"
#include "Violet/Events/KeyEvent.h"
#include "Violet/Renderer/OrthographicCameraController.h"
//#### Vendor ####
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
//#### Renderer ####
#include "Violet/Renderer/Renderer.h"
#include "Violet/Renderer/Renderer2D.h"
#include "Violet/Renderer/Texture.h"
#include "Violet/Renderer/FrameBuffer.h"
//#### Scene ####
#include "Violet/Scene/Scene.h"
#include "Violet/Scene/Entity.h"
#include "Violet/Scene/Components.h"
#include "Violet/Scene/Script.h"
