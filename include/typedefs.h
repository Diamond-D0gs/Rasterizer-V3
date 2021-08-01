#include <stdint.h>

// Requires Clang C Vector Extensions

// Integer Vector Types
typedef int   int2   __attribute__((ext_vector_type(2)));

// Float Vector Types
typedef float float2 __attribute__((ext_vector_type(2)));
typedef float float3 __attribute__((ext_vector_type(3)));
typedef float float4 __attribute__((ext_vector_type(4)));

typedef struct RGB {
	uint8_t r, g, b;
} RGB;

typedef struct Vec2i {
	int x, y;
} Vec2i;

typedef union Vec2f {
	struct {float x, y;};
	struct {float u, v;};
} Vec2f;

typedef union Vec3f {
	struct {float x, y, z;};
	struct {float u, v, w;};
} Vec3f;

typedef struct Vertice {
	Vec3f pos;
	RGB color;
	Vec2i uv;
} Vertice;

typedef struct BBox2Di {
	Vec2i min, max;
} BBox2Di;