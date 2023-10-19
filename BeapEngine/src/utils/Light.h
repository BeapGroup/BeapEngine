#pragma once

#include "../hierarchy/instances/modelobject.h"
#include <glm/glm.hpp>
#include "../renderer/Model.h"

namespace beap
{
	namespace renderer
	{
		enum ELightType
		{
			DIRECTIONAL,
			SPOT,
			POINT
		};

		struct FLightData
		{
			glm::vec3 lightColor;
			float intensity;
			float attenuation;
		};

		class Light : public ModelObject
		{
		public:
			Model* LocalModel;
			ELightType type;
			FLightData data;

			std::string_view InstanceType() const override { return "modelObject"; }


			Light() = default;
			explicit Light(Model* m) : LocalModel(m) {}
			Light(Node<Instance*>* parent, std::string const& instname)
				: ModelObject(parent, instname, new beap::Model("resources/models/plane.gltf")) {}

		};
	}
}