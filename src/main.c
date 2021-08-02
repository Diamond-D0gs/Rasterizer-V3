#include <stdio.h>
#include <stdbool.h>
#include "veclib.h"
#include "typedefs.h"
#include "libbmp.h"

const int WIDTH  = 1024;
const int HEIGHT = 1024;

int EdgeFunction(Vec2i A, Vec2i B, Vec2i P);
BBox2Di CalcBBox2D(const Vec2i* triangle);
Vec3i Barycentric2D(const Vec2i* triangle, const Vec2i P);
void RasterizeTriangle(bmp_img* img, Vertice* verts, int* index, int indexLoc);

int main(void) {
    Vertice verts[4] = {
        {{12, 12, 0}, {255, 0, 0}, {0, 0}},
        {{1012, 12, 0}, {0, 255, 0}, {1, 0}},
        {{12, 1012, 0}, {0, 0, 255}, {0, 1}},
        {{1012, 1012, 0}, {255, 255, 255}, {1, 1}}};
    int index[6] = {0, 3, 2, 0, 1, 3};

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

int EdgeFunction(Vec2i A, Vec2i B, Vec2i P) {
    return ((A.y - B.y)*P.x + (B.x - A.x)*P.y + (A.x*B.y - A.y*B.x));
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

Vec3i Barycentric2D(const Vec2i* triangle, const Vec2i P) {
    Vec3i bary;

    // BCP
    bary.u = EdgeFunction(triangle[1], triangle[2], P);
    // CAP
    bary.v = EdgeFunction(triangle[2], triangle[0], P);
    // ABP
    bary.w = EdgeFunction(triangle[0], triangle[1], P);

    return bary;
}

// Optimizations derived from (https://fgiesen.wordpress.com/2013/02/10/optimizing-the-basic-rasterizer/)
void RasterizeTriangle(bmp_img* img, Vertice* verts, int* index, int indexLoc) {
    Vec2i triangle[3] = {
        (Vec2i){verts[index[indexLoc]].pos.x, verts[index[indexLoc]].pos.y},
        (Vec2i){verts[index[indexLoc+1]].pos.x, verts[index[indexLoc+1]].pos.y},
        (Vec2i){verts[index[indexLoc+2]].pos.x, verts[index[indexLoc+2]].pos.y}};
    // Area is the area of the edge function, or twice the triangle.
    int area = EdgeFunction(triangle[0], triangle[1], triangle[2]);
    // Only operate on the triangle if its area is positive
    if (area > 0) {
        // Calculate the differences
        Vec3i Ystep = {{triangle[1].y - triangle[2].y, triangle[2].y - triangle[0].y, triangle[0].y - triangle[1].y}};
        Vec3i Xstep = {{triangle[2].x - triangle[1].x, triangle[0].x - triangle[2].x, triangle[1].x - triangle[0].x}};
        // Calculate the bounding box
        BBox2Di bBox = CalcBBox2D(triangle);
        // Calculate barycentric coordinates to the minimum corner of the bounding box
        Vec3i row = Barycentric2D(triangle, bBox.min);
        // Rasterize triangle within its bounding box
        for (int y = bBox.min.y; y <= bBox.max.y; ++y) {
            Vec3i bary = row;
            for (int x = bBox.min.x; x <= bBox.max.x; ++x) {
                if ((bary.u | bary.v | bary.w) >= 0) {
                    Vec3f weight = {{bary.u/(float)area}, {bary.v/(float)area}, {bary.w/(float)area}};
                    RGB color = {
                        color.r = (weight.u * verts[index[indexLoc]].color.r) + (weight.v * verts[index[indexLoc+1]].color.r) + (weight.w * verts[index[indexLoc+2]].color.r),
                        color.g = (weight.u * verts[index[indexLoc]].color.g) + (weight.v * verts[index[indexLoc+1]].color.g) + (weight.w * verts[index[indexLoc+2]].color.g),
                        color.b = (weight.u * verts[index[indexLoc]].color.b) + (weight.v * verts[index[indexLoc+1]].color.b) + (weight.w * verts[index[indexLoc+2]].color.b)};
                    bmp_pixel_init (&img->img_pixels[y][x], color.r, color.g, color.b);
                }
                // Step to the right
                Vec3i_add_eq(&bary, Ystep);
            }
            // Step one row
            Vec3i_add_eq(&row, Xstep);
        }
    }
}