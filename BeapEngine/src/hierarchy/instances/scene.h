#pragma once

#include <functional>

#include "gameobject.h"
#include "../../renderer/Shader.h"
#include "../../renderer/ShaderManager.h"

namespace beap {

	class Camera : public Instance {
	public:
		bool FirstClick = false;
		bool IsActive = true;
		glm::f32vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::f32vec2 Viewport;
		float FOV = 90.0;

		void Rotate(glm::f32vec3 const& euler);
		glm::mat4 GetPosition() const;
		glm::mat4 GetView() const;
		glm::f32vec3 GetForward() const;

		std::string_view InstanceType() const override { return "camera"; }
		//TEMPORARY: movement shouldn't be baked into camera functionality
		void Update(GLFWwindow* w, float dt) override;

		Camera(glm::u32vec3 pos, glm::u32vec2 vp) : Position(pos), Viewport(vp) {}
		Camera(Node<Instance*>* parent, std::string const& instname, glm::u32vec3 pos, glm::u32vec2 vp)
			: Instance(parent, instname), Position(pos), Viewport(vp) {}
		explicit Camera(glm::u32vec2 vp) : Viewport(vp) {}

	private:
		glm::f32vec3 FrontVector = glm::f32vec3(0, 0, -1.0f);
	};

	class Scene : public Instance {

	public:
		Camera* ActiveCamera;

		void RenderScene(ShaderManager shaderManager) const;
		void Update(GLFWwindow* w, float dt) override;
		void FindCamera();

		std::string_view InstanceType() const override { return "scene"; }

		Scene(std::string const& instname) : Instance(instname) {}
		Scene(Node<Instance*>* parent, std::string const& instname) : Instance(parent, instname) {}

	};

}