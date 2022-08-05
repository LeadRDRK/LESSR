#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <LESSR/Enums.h>

namespace LESSR
{

struct VertexAttribute
{
    DataType type;
    size_t stride;
    size_t offset;
};

typedef std::unordered_map<std::string, VertexAttribute> VertexLayout;

}