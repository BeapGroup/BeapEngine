#pragma once
#include "instance.h"
#include "../../scripting/beaplua.h"

namespace beap {

	class Script : public Instance {
	public:
		lua::LuaInterpreter LocalLua;
		std::string Source;


		std::string_view InstanceType() const override { return "Script"; }
		void Update(GLFWwindow* w, float dt) override;
		void GetLuaTable(lua_State* L) override;

		Script() = default;
		Script(Node<Instance*>* parent, std::string const& instname, std::string const& source)
			: Instance(parent, instname), Source(source) {
			LocalLua = lua::LuaInterpreter(true, true);
			LocalLua.ExecuteString(Source);
		}
	};

}
