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

}