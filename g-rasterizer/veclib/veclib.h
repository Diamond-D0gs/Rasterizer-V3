#ifndef VECLIB_H
#define VECLIB_H

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

typedef union Vec3i {
	struct {int x, y, z;};
	struct {int u, v, w;};
} Vec3i;

Vec3i Vec3i_add(const Vec3i A, const Vec3i B);
void Vec3i_add_eq(Vec3i* A, const Vec3i B);

#endif