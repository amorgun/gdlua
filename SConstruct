#!/usr/bin/env python
env = SConscript("./godot-cpp/SConstruct")
env.Append(LINKFLAGS=[
    "-Wl,--no-as-needed",
    "-Wl,--no-undefined",
    # "-Wl,--verbose",
])

env.Append(CPPPATH=["luajit/src"])

env.Append(LIBPATH=[Dir("bin").abspath])
env.Append(LIBS=["luajit"])
sources = Glob("*.cpp")

if env["platform"] == "linux":
    lua_lib_name = 'libluajit'
    lua_lib_filename = f'{lua_lib_name}.so'
    lua_lib_result = f"bin/{lua_lib_filename}"
    lua_flags = "TARGET_SYS=Linux"
    copy_actions = [Copy(f'bin/{lua_lib_name}-5.1.so.2', f'luajit/src/{lua_lib_filename}')]
elif env["platform"] == "windows":
    lua_lib_name = "luajit"
    lua_lib_filename = 'lua51.dll'
    lua_lib_result = f"bin/{lua_lib_filename}"
    lua_flags = 'TARGET_SYS=Windows HOST_CC="gcc" CROSS=x86_64-w64-mingw32-'
    copy_actions = [Copy('bin/libluajit.a', f'luajit/src/libluajit-5.1.dll.a')]

lua_build = env.Command(
    target=lua_lib_result,
    source=[],
    action=[
        f'make -C luajit clean',
        f'make -C luajit {lua_flags}',
        *copy_actions,
        Copy(lua_lib_result, f'luajit/src/{lua_lib_filename}'),
    ],
)

library = env.SharedLibrary(
    "bin/libgdlua{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
    source=sources,
)
env.Requires(library, lua_build)
Default(library)
