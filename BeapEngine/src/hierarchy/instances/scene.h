#pragma once

#include "gameobject.h"

namespace beap {

	class camera : public instance {
	public:
		bool active = true;
		glm::u32vec3 position = glm::u32vec3(0);
		glm::u32vec2 viewport;

		void setFOV(int fov);
		void rotate(glm::f32vec3 const& euler);
		glm::mat4 getProjection();
		glm::mat4 getView();
		glm::f32vec3 getForward();

		std::string_view instance_type() const override { return "camera"; }

		camera(glm::u32vec3 pos, glm::u32vec2 vp) : position(pos), viewport(vp) {}
		explicit camera(glm::u32vec2 vp) : viewport(vp) {}

	private:
		glm::f32vec3 front_vector = glm::f32vec3(0, 0, -1);
	};

	class scene : public instance {

	public:
		camera* active_camera;
		glm::mat4 world_matrix;


	};

}