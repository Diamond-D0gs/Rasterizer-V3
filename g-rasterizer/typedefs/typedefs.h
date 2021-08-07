#include <stdint.h>
#include "veclib.h"

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

typedef int int3 __attribute__((ext_vector_type(3)));
typedef float float3 __attribute__((ext_vector_type(3)));

typedef struct RGB {
	uint8_t r, g, b;
} RGB;

typedef struct Vertice {
	Vec3f pos;
	RGB color;
	Vec2i uv;
} Vertice;

typedef struct BBox2Di {
	Vec2i min, max;
} BBox2Di;

#endif