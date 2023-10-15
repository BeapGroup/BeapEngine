#include "modelobject.h"

namespace beap {

	void modelObject::apply_to_meshes(std::function<void(Mesh*)> const& apply) {
		for (int i = 0; i < model->meshes.size(); i++) {
			apply(&model->meshes.at(i));
		}
	}

}