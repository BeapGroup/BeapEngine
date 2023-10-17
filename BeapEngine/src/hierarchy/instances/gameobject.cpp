#include "gameobject.h"

namespace beap {

	void GameObject::ResetTransform() {
		Position = glm::u32vec3(0);
		Rotation = glm::f32vec3(0);
		Scale = glm::u32vec3(0);
	}

	void GameObject::CopyTransform(GameObject const* gbj) {
		Position = gbj->Position;
		Rotation = gbj->Rotation;
		Scale = gbj->Scale;
	}

	void GameObject::Move(glm::f32vec3 shift) {
		Position += shift;
	}

	void GameObject::SetPosition(glm::f32vec3 pos) {
		Position = pos;
	}

	void GameObject::SetRotation(glm::f32vec3 euler) {
		Rotation = euler;
	}

	void GameObject::SetScale(glm::f32vec3 scalar) {
		Scale = scalar;
	}
}