#include "gameobject.h"

namespace beap {

	void gameObject::reset_transform() {
		position = glm::u32vec3(0);
		rotation = glm::f32vec3(0);
		scale = glm::u32vec3(0);
	}

	void gameObject::copy_transform(gameObject const* gbj) {
		position = gbj->position;
		rotation = gbj->rotation;
		scale = gbj->scale;
	}

	void gameObject::move(glm::f32vec3 shift) {
		position += shift;
	}

	void gameObject::setPosition(glm::f32vec3 pos) {
		position = pos;
	}

	void gameObject::setRotation(glm::f32vec3 euler) {
		rotation = euler;
	}

	void gameObject::setScale(glm::f32vec3 scalar) {
		scale = scalar;
	}
}