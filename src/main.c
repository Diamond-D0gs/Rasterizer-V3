#include <stdio.h>
#include <stdbool.h>
#include "typedefs.h"
#include "libbmp.h"

const int WIDTH  = 1024;
const int HEIGHT = 1024;

float EdgeFunction(Vec2i A, Vec2i B, Vec2i P);
BBox2Di CalcBBox2D(const Vec2i* triangle);
Vec3f Barycentric2D(const Vec2i* triangle, const float area, const Vec2i P);
void RasterizeTriangle(bmp_img* img, Vertice* verts, int* index, int indexLoc);

int main(void) {
    Vertice verts[4] = {
        {{12, 12, 0}, {255, 0, 0}, {0, 0}},
        {{1012, 12, 0}, {0, 255, 0}, {1, 0}},
        {{12, 1012, 0}, {0, 0, 255}, {0, 1}},
        {{1012, 1012, 0}, {255, 255, 255}, {1, 1}}};
    int index[6] = {0, 2, 3, 0, 3, 1};

    bmp_img img;
    bmp_img_init_df (&img, WIDTH, HEIGHT);

    for (int i = 0; i < (sizeof(index)/sizeof(int)); i += 3) {
        RasterizeTriangle(&img, verts, index, i);
    }
    // Rasterization complete
	
	bmp_img_write (&img, "test.bmp");
	bmp_img_free (&img);

    return 0;
}

float EdgeFunction(Vec2i A, Vec2i B, Vec2i P) {
    return ((P.x - A.x) * (B.y - A.y) - (P.y - A.y) * (B.x - A.x));
}

BBox2Di CalcBBox2D(const Vec2i* triangle) {
    BBox2Di bBox = {triangle[0], triangle[0]};

    for (int i = 0; i < 3; ++i) {
        if (triangle[i].x < bBox.min.x)
            bBox.min.x = triangle[i].x;
        else if (triangle[i].x > bBox.max.x)
            bBox.max.x = triangle[i].x;

        if (triangle[i].y < bBox.min.y)
            bBox.min.y = triangle[i].y;
        else if (triangle[i].y > bBox.max.y)
            bBox.max.y = triangle[i].y;   
    }

    return bBox;
}

// Area is the area of the edge function, or twice the triangle.
Vec3f Barycentric2D(const Vec2i* triangle, const float area, const Vec2i P) {
    Vec3f bary;

    // BCP
    bary.u = EdgeFunction(triangle[1], triangle[2], P) / area;
    // CAP
    bary.v = EdgeFunction(triangle[2], triangle[0], P) / area;
    // ABP
    bary.w = EdgeFunction(triangle[0], triangle[1], P) / area;

    return bary;
}

void RasterizeTriangle(bmp_img* img, Vertice* verts, int* index, int indexLoc) {
    Vec2i triangle[3] = {
        (Vec2i){verts[index[indexLoc]].pos.x, verts[index[indexLoc]].pos.y},
        (Vec2i){verts[index[indexLoc+1]].pos.x, verts[index[indexLoc+1]].pos.y},
        (Vec2i){verts[index[indexLoc+2]].pos.x, verts[index[indexLoc+2]].pos.y}};

    // Area is the area of the edge function, or twice the triangle.
    float area = EdgeFunction(triangle[0], triangle[1], triangle[2]);
    if (area > 0) {
        BBox2Di bBox = CalcBBox2D(triangle);
        // Rasterize triangle within its bounding box
        for (int y = bBox.min.y; y <= bBox.max.y; ++y) {
            for (int x = bBox.min.x; x <= bBox.max.x; ++x) {
                Vec3f bary = Barycentric2D(triangle, area, (Vec2i){x, y});
                if (bary.u >= 0 && bary.v >= 0 && bary.w >= 0) {
                    RGB color = {
                        color.r = (bary.u * verts[index[indexLoc]].color.r) + (bary.v * verts[index[indexLoc+1]].color.r) + (bary.w * verts[index[indexLoc+2]].color.r),
                        color.g = (bary.u * verts[index[indexLoc]].color.g) + (bary.v * verts[index[indexLoc+1]].color.g) + (bary.w * verts[index[indexLoc+2]].color.g),
                        color.b = (bary.u * verts[index[indexLoc]].color.b) + (bary.v * verts[index[indexLoc+1]].color.b) + (bary.w * verts[index[indexLoc+2]].color.b)};
                    bmp_pixel_init (&img->img_pixels[y][x], color.r, color.g, color.b);
                }
            }
        }
    }
}