#ifndef GDLUASTATE_H
#define GDLUASTATE_H

#include <godot_cpp/classes/ref_counted.hpp>
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace godot {

class GDLuaState : public RefCounted {
	GDCLASS(GDLuaState, RefCounted)

private:
	lua_State *L;
	Variant box_lua_value(const int idx) const;

protected:
	static void _bind_methods();
	
public:
	GDLuaState();
	virtual ~GDLuaState() override;
	Error dostring(const String &s);
	Variant pop_value();
	Variant getglobal(const String &name);
};

}

#endif