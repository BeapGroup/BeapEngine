#pragma once

#include <functional>

#include "gameobject.h"
#include "../../renderer/Shader.h"

namespace beap {

	class camera : public instance {
	public:
		bool active = true;
		glm::f32vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::f32vec2 viewport;
		float FOV = 90.0;

		void rotate(glm::f32vec3 const& euler);
		glm::mat4 getProjection() const;
		glm::mat4 getView() const;
		glm::f32vec3 getForward() const;

		std::string_view instance_type() const override { return "camera"; }
		//TEMPORARY: movement shouldn't be baked into camera functionality
		void update(GLFWwindow* w, float dt) override;

		camera(glm::u32vec3 pos, glm::u32vec2 vp) : position(pos), viewport(vp) {}
		camera(node<instance*>* parent, std::string const& instname, glm::u32vec3 pos, glm::u32vec2 vp)
			: instance(parent, instname), position(pos), viewport(vp) {}
		explicit camera(glm::u32vec2 vp) : viewport(vp) {}

	private:
		glm::f32vec3 front_vector = glm::f32vec3(0, 0, -1.0f);
	};

	class scene : public instance {

	public:
		camera* active_camera;

		void render_scene(Shader s) const;
		void update(GLFWwindow* w, float dt) override;
		void find_camera();

		std::string_view instance_type() const override { return "scene"; }

		scene(std::string const& instname) : instance(instname) {}
		scene(node<instance*>* parent, std::string const& instname) : instance(parent, instname) {}
	};

}