#include "modelobject.h"

namespace beap {

	void ModelObject::ApplyToMeshes(std::function<void(Mesh*)> const& apply) {
		for (auto& mesh : LocalModel->meshes) {
			apply(&mesh);
		}
	}

	void ModelObject::Move(glm::f32vec3 shift) {
		Position += shift;
		ApplyToMeshes([shift](Mesh* m) {
			m->translation += shift;
			});
	}

	void ModelObject::SetPosition(glm::f32vec3 pos) {
		Position = pos;
		ApplyToMeshes([pos](Mesh* m) {
			m->translation = pos;
			});
	}
	
	void ModelObject::SetRotation(glm::f32vec3 euler) {
		Rotation = euler;
		ApplyToMeshes([euler](Mesh* m) {
			m->eulerRotation = euler;
			});
	}

	void ModelObject::SetScale(glm::f32vec3 scalar) {
		Scale = scalar;
		ApplyToMeshes([scalar](Mesh* m) {
			m->scale = scalar;
			});
	}
}