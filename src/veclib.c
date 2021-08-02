#include "veclib.h"

Vec3i Vec3i_add(const Vec3i A, const Vec3i B) {
    return ((Vec3i){A.x + B.x, A.y + B.y, A.z + B.z});
}

void Vec3i_add_eq(Vec3i* A, const Vec3i B) {
    A->x += B.x;
    A->y += B.y;
    A->z += B.z;
}