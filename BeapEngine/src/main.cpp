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
#include "hierarchy/instances/Script.h"

#include "BeapEditor.h"

#include "utils/Light.h"

#include "renderer/ShaderManager.h"

#include "scripting/beaplua.h"

int SCR_WIDTH = 1280;
int SCR_HEIGHT = 720;

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

	auto interp = beap::lua::LuaInterpreter(true, true);
	interp.ExecuteString("print(BeapEngine.Vector2:FromNum(1,0):Rotate(math.pi / 2))");

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
	beap::Shader billboardShader("resources/shaders/default.vert", "resources/shaders/default.frag");

	shaderManager.AddShader(&defaultShader);
	shaderManager.AddShader(&billboardShader);

	beap::ModelObject monker(scene1.in_tree, "monker", new beap::Model("resources/models/monkey.gltf"));
	beap::ModelObject generations(scene1.in_tree, "generations", new beap::Model("resources/models/generations.gltf"));
	auto c = beap::Model::Cube(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1));
	beap::ModelObject cuber(scene1.in_tree, "planer", &c);
	
	beap::Script matthew(monker.in_tree, "matthew-script", R"( function update(parent, dt) print(GetInstance(FindChild(parent, "matthew-script")).source) end )");

	cuber.Move(glm::vec3(.5f, -.5f, -.5f));

	cuber.ApplyToMeshes([&defaultShader](beap::Mesh* m) {
		m->shader = &defaultShader;
		m->SetupTexture("resources/textures/mayro.png");
		});
	monker.ApplyToMeshes([&defaultShader](beap::Mesh* m) {
		m->shader = &defaultShader;
		m->SetupTexture("resources/textures/mayro.png");
		m->eulerRotation = glm::vec3(0, -90, 0);
		});

	generations.ApplyToMeshes([&defaultShader](beap::Mesh* m) {
		m->shader = &defaultShader;
		m->SetupTexture("resources/textures/genplayer.png");
		});

	auto light = beap::renderer::Light(scene1.in_tree, "Light");
	light.ApplyToMeshes([&defaultShader](beap::Mesh* m) {
		m->shader = &defaultShader;
		m->SetupTexture("resources/textures/mayro.png");
		});


	

	beap::editor::BeapEditor editor(window, &scene1);
	editor.Init();



	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		
		double now = glfwGetTime();
		float dt = now - lastTime;
		editor.PreRender();


		//// TEMPORARY CODE

		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


		//// END TEMPORARY CODE

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glClearColor(0.2f, 0.7f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		defaultShader.setVec3("lightPos", light.Position);
		defaultShader.setVec4("lightCol", light.data.lightColor);

		camera1.Viewport = glm::vec2(SCR_WIDTH, SCR_HEIGHT);

		scene1.Update(window, dt);
		scene1.RenderScene(shaderManager);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(0.2f, 0.7f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		editor.EditorLoop(textureColorbuffer);
		editor.Render();

		glfwSwapBuffers(window);
		glfwPollEvents();

		lastTime = now;
	}

	glfwTerminate();
	return 0;
}