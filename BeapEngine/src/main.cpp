#include <iostream>


#define GLFW_INCLUDE_NONE //disable glfw loading opengl, since glad already does that

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Mesh.h"
#include "math/tree.h"
#include "Model.h"

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

int main() {

	auto root = node<int>(5);
	auto cooper = node<int>(7);
	root.add_child(&cooper);
	root.traverse([](auto kid) { std::cout << kid->data << std::endl; });

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

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(window, on_framebuf_resize);

	
	// TODO : Shader pre-processing system and manager
	Shader defaultShader("resources/shaders/default.vert","resources/shaders/default.frag");
	Model cube("resources/models/monkey.gltf");
	for (int i = 0; i < cube.meshes.size(); i++)
	{
		cube.meshes.at(i).shader = &defaultShader;
		cube.meshes.at(i).SetupTexture("resources/textures/mayro.png");
		cube.meshes.at(i).eulerRotation = glm::vec3(0, -90, 0);
	}


	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		double now = glfwGetTime();
		float dt = now - lastTime;
		glClearColor(0.2f, 0.7f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		defaultShader.use();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		defaultShader.setMat4("projection", projection);

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		defaultShader.setMat4("view", view);

		cube.Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

		lastTime = now;
	}

	glfwTerminate();
	return 0;
}