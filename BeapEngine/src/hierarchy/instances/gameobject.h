#pragma once

#include <glm/glm.hpp>
#include "instance.h"

namespace beap {

	class gameObject : public instance {
	public:
		glm::f32vec3 position;
		glm::f32vec3 rotation = glm::f32vec3(0);
		glm::f32vec3 scale = glm::u32vec3(1);

		std::string_view instance_type() const override { return "gameObject"; }
		void reset_transform();
		void copy_transform(gameObject const* gbj);

		virtual void render() = 0;
		virtual void move(glm::f32vec3 shift);
		virtual void setPosition(glm::f32vec3 pos);
		virtual void setRotation(glm::f32vec3 euler);
		virtual void setScale(glm::f32vec3 scalar);

		gameObject() = default;
		gameObject(glm::u32vec3 pos, glm::f32vec3 rot, glm::u32vec3 scl)
			: position(pos), rotation(rot), scale(scl) {}
		gameObject(glm::u32vec3 pos, const std::string& instname) 
			: instance(instname), position(pos) {}
		gameObject(glm::u32vec3 pos, const std::string& instname, node<instance*>* parent)
			: instance(parent, instname), position(pos) {}
	};
}