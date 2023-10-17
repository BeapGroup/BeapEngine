#pragma once

#include "thirdparty/imgui/imgui.h"
#include "thirdparty/imgui/imgui_impl_glfw.h"
#include "thirdparty/imgui/imgui_impl_opengl3.h"


#include "hierarchy/instances/scene.h"

namespace beap
{
	namespace editor
	{
		class BeapEditor
		{
			public:
				GLFWwindow* window;
				beap::Scene* scene;
				BeapEditor(GLFWwindow* window, beap::Scene* scene);
				void Init();
				void PreRender();
				void EditorLoop();
				void Render();
				void CleanUp();
		};
	}
}
