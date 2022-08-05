#pragma once
#include <cstdint>

namespace LESSR
{
struct ImageData
{
    uint32_t width;
    uint32_t height;
    uint8_t* pixels;
};
}