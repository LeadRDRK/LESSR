# Triangle
add_executable(triangle Triangle.cpp)
target_link_libraries(triangle PRIVATE lessr)

# Textured tetrahedron with perspective projection
add_executable(tetrahedron Tetrahedron.cpp)
target_link_libraries(tetrahedron PRIVATE lessr)

# Suzanne
add_executable(suzanne Suzanne.cpp)
target_link_libraries(suzanne PRIVATE lessr)

# Tetrahedron but with Lua shaders
add_executable(luashaders LuaShaders.cpp)
target_link_libraries(luashaders PRIVATE lessr)