#include "scene.h"
#include "modelobject.h"
#include <glm/gtx/rotate_vector.hpp>

namespace beap {

	void camera::rotate(glm::f32vec3 const& euler) {
		front_vector = glm::rotateX(front_vector, euler.x);
		front_vector = glm::rotateY(front_vector, euler.y);
		front_vector = glm::rotateZ(front_vector, euler.z);
	}

	glm::mat4 camera::getProjection() const {
		return glm::perspective(glm::radians(FOV / 2), viewport.x / viewport.y, 0.1f, 500.0f);
	}

	glm::mat4 camera::getView() const {
		return glm::lookAt(glm::f32vec3(position), glm::f32vec3(position) + front_vector, glm::f32vec3(0.0, 1.0f, 0.0));
	}

	glm::f32vec3 camera::getForward() const {
		return front_vector;
	}

	void camera::update(GLFWwindow* w, float dt) {
		if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS)
			position += front_vector * dt;
		if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS)
			position -= front_vector * dt;
		if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS)
			position -= glm::normalize(glm::cross(front_vector, glm::vec3(0,1,0))) * dt;
		if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS)
			position += glm::normalize(glm::cross(front_vector, glm::vec3(0, 1, 0))) * dt;
	}

	void scene::render_scene(Shader s) const {
		s.use();
		s.setMat4("projection", active_camera->getProjection());
		s.setMat4("view", active_camera->getView());
		for (auto child : getchildren()) {
			instance* child_instance = child->data;
			if (child_instance->instance_type() == "modelObject") {
				((modelObject*)child_instance)->render();
			}
		}
	}

	void scene::update(GLFWwindow* w, float dt) {
		for (auto& instnode : getchildren()) {
			instnode->data->update(w, dt);
		}
	}

	void scene::find_camera() {
		for (auto child : getchildren()) {
			instance* child_instance = child->data;
			if (child_instance->instance_type() == "camera") {
				active_camera = (camera*)child_instance;
			}
		}

	}
}