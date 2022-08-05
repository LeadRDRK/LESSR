#pragma once

namespace LESSR
{

enum DataType
{
    FLOAT,
    INT,
    UINT,
    SHORT,
    USHORT,
    BYTE,
    UBYTE,
    VECTOR2,
    VECTOR3,
    VECTOR4,
    COLOR3,
    COLOR4,
    MATRIX4,
    IMAGEDATA
};

enum BufferType
{
    ARRAY_BUFFER,
    ELEMENT_ARRAY_BUFFER
};

enum PrimitiveType
{
    POINTS,
    TRIANGLES
};

enum Capability
{
    BLEND,
    DEPTH_TEST,
    CULL_FACE
};

enum Faces
{
    FRONT,
    BACK,
    FRONT_AND_BACK
};

enum Winding
{
    CCW,
    CW
};

}