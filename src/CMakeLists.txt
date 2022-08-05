set(LESSR_INC ../include/LESSR)

set(LESSR_HEADERS
    ${LESSR_INC}/Color3.h
    ${LESSR_INC}/Color4.h
    ${LESSR_INC}/Context.h
    ${LESSR_INC}/DefaultProgram.h
    ${LESSR_INC}/Enums.h
    ${LESSR_INC}/ImageData.h
    ${LESSR_INC}/MathUtils.h
    ${LESSR_INC}/Matrix4.h
    ${LESSR_INC}/Program.h
    ${LESSR_INC}/ShaderContext.h
    ${LESSR_INC}/Vector2.h
    ${LESSR_INC}/Vector3.h
    ${LESSR_INC}/Vector4.h
    ${LESSR_INC}/VertexAttribute.h
)

set(LESSR_SOURCES
    Color3.cpp
    Color4.cpp
    Context.cpp
    DefaultProgram.cpp
    Matrix4.cpp
    Program.cpp
    ShaderContext.cpp
    Vector2.cpp
    Vector3.cpp
    Vector4.cpp
)

if(NOT LESSR_DISABLE_LUA)
    set(LESSR_HEADERS
        ${LESSR_HEADERS}
        ${LESSR_INC}/LuaLib.h
        ${LESSR_INC}/LuaProgram.h
    )

    set(LESSR_SOURCES
        ${LESSR_SOURCES}
        LuaLib.cpp
        LuaProgram.cpp
        LuaUtils.h
    )

    find_package(Lua REQUIRED)
endif()

add_library(lessr ${LESSR_HEADERS} ${LESSR_SOURCES})
target_include_directories(lessr PUBLIC ../include ${LUA_INCLUDE_DIR})
target_link_libraries(lessr PRIVATE ${LUA_LIBRARIES})
set_property(TARGET lessr PROPERTY CXX_STANDARD 11)

if(NOT LESSR_DISABLE_OPENMP)
    find_package(OpenMP)
    if(OpenMP_CXX_FOUND)
        target_link_libraries(lessr PUBLIC OpenMP::OpenMP_CXX)
    endif()
endif()