cmake_minimum_required(VERSION 3.9.0)
project(lessr
    LANGUAGES CXX
)

option(LESSR_BUILD_TESTS "Build test programs" ON)
option(LESSR_DISABLE_LUA "Disable Lua support" OFF)
option(LESSR_DISABLE_OPENMP "Disable OpenMP support" OFF)

add_subdirectory(src)

if(LESSR_BUILD_TESTS)
    add_subdirectory(tests)
endif()