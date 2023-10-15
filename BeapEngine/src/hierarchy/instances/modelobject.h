#pragma once

#include "gameobject.h"
#include "../../renderer/Model.h"

namespace beap {

	class modelObject : public gameObject {

	public:
		Model* model;

		void render() override { model->Draw(); }
		std::string_view instance_type() const override { return "modelObject"; }
		void move(glm::f32vec3 shift) override;
		void setPosition(glm::f32vec3 pos) override;

		void apply_to_meshes(std::function<void(Mesh*)> const& apply);
		void apply_shader(Shader s) { apply_to_meshes([s](Mesh* m) {*m->shader = s; }); }

		modelObject() = default;
		explicit modelObject(Model* m) : model(m) {}
		modelObject(node<instance*>* parent, std::string const& instname, Model* m) 
			: gameObject(glm::vec3(0), instname, parent), model(m) {}
	};

}