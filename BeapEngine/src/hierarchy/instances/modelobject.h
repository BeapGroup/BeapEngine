#pragma once

#include "gameobject.h"
#include "../../renderer/Model.h"

namespace beap {

	class ModelObject : public GameObject {

	public:
		Model* LocalModel;

		void Render() override { LocalModel->Draw(); }
		std::string_view InstanceType() const override { return "modelObject"; }
		void Move(glm::f32vec3 shift) override;
		void SetPosition(glm::f32vec3 pos) override;
		void SetRotation(glm::f32vec3 euler) override;
		void SetScale(glm::f32vec3 scalar) override;

		void ApplyToMeshes(std::function<void(Mesh*)> const& apply);
		void ApplyShader(Shader s) { ApplyToMeshes([s](Mesh* m) {*m->shader = s; }); }

		ModelObject() = default;
		explicit ModelObject(Model* m) : LocalModel(m) {}
		ModelObject(Node<Instance*>* parent, std::string const& instname, Model* m) 
			: GameObject(glm::vec3(0), instname, parent), LocalModel(m) {}
	};

}