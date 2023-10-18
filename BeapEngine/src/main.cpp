#include <iostream>


#define GLFW_INCLUDE_NONE //disable glfw loading opengl, since glad already does that

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer/Shader.h"
#include "renderer/Mesh.h"
#include "math/tree.h"
#include "renderer/Model.h"

#include "hierarchy/instances/instance.h"
#include "hierarchy/instances/scene.h"
#include "hierarchy/instances/modelobject.h"

#include "BeapEditor.h"

#include "utils/Light.h"

#include "renderer/ShaderManager.h"

int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

void on_framebuf_resize(GLFWwindow*, int width, int height) {
	glViewport(0, 0, width, height);
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// TEMP
glm::vec3 lightPos = glm::vec3(3.0f, 0.0f, 0.0f);

int main() {

	std::cout << "Initialising GLFW, GLAD and Window!" << std::endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__ // get real
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Beap Engine", NULL, NULL);
	if (window == NULL) {
		std::cout << "GL window creation failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD init failed" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	//enable blending (aka alpha channel in shaders)
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(window, on_framebuf_resize);

	beap::Instance root("root");
	beap::Instance::CreateHierarchy(&root);

	beap::Scene scene1(root.in_tree, "scene 1");
	beap::Camera camera1(scene1.in_tree, "camera 1", glm::vec3(0,0,3), glm::vec2(SCR_WIDTH, SCR_HEIGHT));
	scene1.FindCamera();

	beap::ShaderManager shaderManager;

	beap::Shader defaultShader("resources/shaders/default.vert", "resources/shaders/default.frag");
	beap::Shader billboardShader("resources/shaders/billboard.vert", "resources/shaders/default.frag");

	shaderManager.AddShader(&defaultShader);
	shaderManager.AddShader(&billboardShader);

	beap::ModelObject monker(scene1.in_tree, "monker", new beap::Model("resources/models/monkey.gltf"));
	auto c = beap::Model::Cube(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1));
	beap::ModelObject cuber(scene1.in_tree, "planer", &c);
	
	cuber.Move(glm::vec3(.5f, -.5f, -.5f));

	cuber.ApplyToMeshes([&defaultShader](beap::Mesh* m) {
		m->shader = &defaultShader;
		m->SetupTexture("resources/textures/mayro.png");
		//m->eulerRotation = glm::vec3(0, -90, 0);
		});
	monker.ApplyToMeshes([&defaultShader](beap::Mesh* m) {
		m->shader = &defaultShader;
		m->SetupTexture("resources/textures/mayro.png");
		m->eulerRotation = glm::vec3(0, -90, 0);
		});

	auto light = beap::renderer::Light(scene1.in_tree, "Light");
	light.ApplyToMeshes([&billboardShader](beap::Mesh* m) {
		m->shader = &billboardShader;
		m->SetupTexture("resources/textures/mayro.png");
		});

	light.Move(glm::vec3(-2, 0, 0));
	


	

	beap::editor::BeapEditor editor(window, &scene1);
	editor.Init();

	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		
		double now = glfwGetTime();
		float dt = now - lastTime;
		editor.PreRender();


		glClearColor(0.2f, 0.7f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		defaultShader.setVec3("lightPos", lightPos);

		camera1.Viewport = glm::vec2(SCR_WIDTH, SCR_HEIGHT);

		scene1.Update(window, dt);
		scene1.RenderScene(shaderManager);
		
		editor.EditorLoop();
		editor.Render();

		glfwSwapBuffers(window);
		glfwPollEvents();

		lastTime = now;
	}

	glfwTerminate();
	return 0;
}