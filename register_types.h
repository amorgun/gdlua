#ifndef GDLUA_REGISTER_TYPES_H
#define GDLUA_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void initialize_gdlua(ModuleInitializationLevel p_level);
void uninitialize_gdlua(ModuleInitializationLevel p_level);

#endif // GDLUA_REGISTER_TYPES_H