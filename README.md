# LESSR
#### Lead's Embedded Scriptable Software Renderer
The goal of this project is to create a somewhat scriptable software renderer for (embedded) systems without dedicated rendering hardware, without using OpenGL or the likes. It also serves as a way for me to understand how 3D rendering works better.

This project is mostly intended for research only, however actual use is plausible. Keep in mind that this is not quite fast and could definitely use some optimizations.
# Usage
LESSR can be directly integrated in your own project using CMake. There are a few build options available, check the root CMakeLists for more details. OpenMP will be enabled automatically if available.
# API
The API is modelled after OpenGL. If you know how to use OpenGL, chances are you probably know how to use LESSR already. However, objects that needed to be created on the "server side" (such as shader programs, buffers, etc.) are now managed by the client itself and passed to the library as raw pointers.

Additionally, data types that composes of multiple values of other types (such as Vector3) have their own type instead. This is to enforce type safety between Lua and C++. The shader interface is based on GLSL 1.10 and uses attribute, varying and uniform as storage qualifiers.

There is no documentation yet. Check the headers for more info (you might wanna look at [Context.h](include/LESSR/Context.h) first)
# Lua Scripting
[LuaLib::openLibs](include/LESSR/LuaLib.h) can be used to load LESSR's Lua bindings. Scripting support for shaders is available through the [LuaProgram](include/LESSR/LuaProgram.h) class. An example is provided in [tests/LuaShaders.cpp](tests/LuaShaders.cpp)

Currently, the Context class is not implemented in Lua and might never be due to its unsafe nature. Therefore, LESSR cannot be used entirely from Lua. You will need to write a library for that yourself.
# License
Licensed under the [MIT License](LICENSE)