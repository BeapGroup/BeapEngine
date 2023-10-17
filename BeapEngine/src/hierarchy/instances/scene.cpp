#include "scene.h"
#include "modelobject.h"
#include <glm/gtx/rotate_vector.hpp>

namespace beap {

	void Camera::Rotate(glm::f32vec3 const& euler) {
		FrontVector = glm::rotateX(FrontVector, euler.x);
		FrontVector = glm::rotateY(FrontVector, euler.y);
		FrontVector = glm::rotateZ(FrontVector, euler.z);
	}

	glm::mat4 Camera::GetPosition() const {
		return glm::perspective(glm::radians(FOV / 2), Viewport.x / Viewport.y, 0.1f, 500.0f);
	}

	glm::mat4 Camera::GetView() const {
		return glm::lookAt(glm::f32vec3(Position), glm::f32vec3(Position) + FrontVector, glm::f32vec3(0.0, 1.0f, 0.0));
	}

	glm::f32vec3 Camera::GetForward() const {
		return FrontVector;
	}

	void Camera::Update(GLFWwindow* w, float dt) {
		if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS)
			Position += FrontVector * dt;
		if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS)
			Position -= FrontVector * dt;
		if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS)
			Position -= glm::normalize(glm::cross(FrontVector, glm::vec3(0,1,0))) * dt;
		if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS)
			Position += glm::normalize(glm::cross(FrontVector, glm::vec3(0, 1, 0))) * dt;
	}

	void Scene::RenderScene(Shader s) const {
		s.use();
		s.setMat4("projection", ActiveCamera->GetPosition());
		s.setMat4("view", ActiveCamera->GetView());
		for (auto child : GetChildren()) {
			Instance* child_instance = child->Contents;
			if (child_instance->InstanceType() == "modelObject") {
				((ModelObject*)child_instance)->Render();
			}
		}
	}

	void Scene::Update(GLFWwindow* w, float dt) {
		for (auto& instnode : GetChildren()) {
			instnode->Contents->Update(w, dt);
		}
	}

	void Scene::FindCamera() {
		for (auto child : GetChildren()) {
			Instance* child_instance = child->Contents;
			if (child_instance->InstanceType() == "camera") {
				ActiveCamera = (Camera*)child_instance;
			}
		}

	}
}