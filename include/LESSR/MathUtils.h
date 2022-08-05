#pragma once
#include <cmath>

#define MATH_LERP(v0, v1, t) (1 - t) * v0 + t * v1
#define MATH_PI              3.14159265358979323846f
#define MATH_PIOOVER2        1.57079632679489661923f
#define MATH_EPSILON         0.000001f
#define MATH_DEG_TO_RAD(x)   ((x) * 0.0174532925f)
#define MATH_RAD_TO_DEG(x)   ((x)* 57.29577951f)
#define MATH_CLAMP(v, lo, hi) std::max(lo, std::min(v, hi))