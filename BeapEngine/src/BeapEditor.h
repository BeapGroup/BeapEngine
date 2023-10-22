#pragma once

#include "thirdparty/imgui/imgui.h"
#include "thirdparty/imgui/imgui_impl_glfw.h"
#include "thirdparty/imgui/imgui_impl_opengl3.h"
#include "renderer/ShaderManager.h"
#include "utils/Light.h"

#include "hierarchy/instances/scene.h"

namespace beap
{
	namespace editor
	{
		class BeapEditor
		{
			// TEMPORARY SHADER MANAGER STUFF. WILL MAKE SEPERATE EDITOR LATER
		private:
			beap::ShaderManager* manager;
			beap::shaders::generation::FShaderGenerationSettings settings;

			Instance* selectedInstance;
		public:
				GLFWwindow* window;
				beap::Scene* scene;
				BeapEditor(GLFWwindow* window, beap::Scene* scene);
				void Init();
				void PreRender();
				void EditorLoop(unsigned int fboTexture);
				void RenderShaderManagerEditor();
				void Render();
				void CleanUp();
		};
	}
}
