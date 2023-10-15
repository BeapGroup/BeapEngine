#include "modelobject.h"

namespace beap {

	void modelObject::apply_to_meshes(std::function<void(Mesh*)> const& apply) {
		for (auto& mesh : model->meshes) {
			apply(&mesh);
		}
	}

	void modelObject::move(glm::f32vec3 shift) {
		position += shift;
		apply_to_meshes([shift](Mesh* m) {
			m->translation += shift;
			});
	}

	void modelObject::setPosition(glm::f32vec3 pos) {
		position = pos;
		apply_to_meshes([pos](Mesh* m) {
			m->translation = pos;
			});
	}
	
	void modelObject::setRotation(glm::f32vec3 euler) {
		rotation = euler;
		apply_to_meshes([euler](Mesh* m) {
			m->eulerRotation = euler;
			});
	}

	void modelObject::setScale(glm::f32vec3 scalar) {
		scale = scalar;
		apply_to_meshes([scalar](Mesh* m) {
			m->scale = scalar;
			});
	}
}