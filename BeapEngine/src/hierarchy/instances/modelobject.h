#pragma once

#include "gameobject.h"
#include "../../renderer/Model.h"

namespace beap {

	class modelObject : public gameObject {

	public:
		Model* model;

		void render() override { model->Draw(); }
		std::string_view instance_type() const override { return "modelObject"; }

		modelObject() = default;
		explicit modelObject(Model* m) : model(m) {}
	};

}