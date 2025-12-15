#include "gdlua.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GDLuaState::_bind_methods() {
	ClassDB::bind_method(D_METHOD("dostring", "s"), &GDLuaState::dostring);
	ClassDB::bind_method(D_METHOD("pop_value"), &GDLuaState::pop_value);
	ClassDB::bind_method(D_METHOD("getglobal", "name"), &GDLuaState::getglobal);
}

GDLuaState::GDLuaState() {
	L = luaL_newstate();
	luaopen_jit(L);
}

GDLuaState::~GDLuaState() {
	lua_close(L);
}

Error GDLuaState::dostring(const String &s) {
	return (luaL_dostring(L, s.utf8().get_data()) == LUA_OK) ? Error::OK : Error::FAILED;
}

Variant GDLuaState::box_lua_value(const int idx) const {
	int t = lua_type(L, idx);
	switch (t) {
		case LUA_TNIL:
			return nullptr;
		case LUA_TSTRING:
			return lua_tostring(L, idx);
		case LUA_TBOOLEAN:
			return static_cast<bool>(lua_toboolean(L, idx));
		case LUA_TNUMBER: {
			lua_Integer ival = lua_tointeger(L, idx);
			lua_Number nval = lua_tonumber(L, idx);
			if (ival == nval) {
				return ival;
			}
			return nval;
		}
		case LUA_TTABLE: {
			const int abs_idx = (idx >= 0) ? idx : (lua_gettop(L) + idx + 1);
			const size_t len = lua_objlen(L, abs_idx);
			bool is_array = true;
			Array arr = Array();
			Dictionary dict = Dictionary();
			arr.resize(len);
			lua_pushnil(L);
			while (lua_next(L, abs_idx) != 0) {
				if (is_array and lua_type(L, -2) != LUA_TNUMBER) {
					is_array = false;
				}
				const Variant &value = box_lua_value(-1);
				lua_pushvalue(L, -2);
				const Variant &key = box_lua_value(-1);  // create a copy to safely call tostring
				if (is_array) {
					if (key.get_type() != Variant::Type::INT) {
						is_array = false;
					}
					else {
						const int key_int = key;
						if (key_int < 1 || key_int > len) {
							is_array = false;
						}
						else {
							arr[key_int - 1] = value;
						}
					}

				}
				dict[std::move(key)] = std::move(value);
				lua_pop(L, 2);
			}
			if (is_array) {
				return arr;
			}
			return dict;
		}
		default:  /* other values */
			return lua_typename(L, idx);
	}
}

Variant GDLuaState::pop_value() {
	Variant result = box_lua_value(-1);
	lua_remove(L, -1);
	return result;
}

Variant GDLuaState::getglobal(const String &name) {
	lua_getglobal(L, name.utf8().get_data());
	return pop_value();
}