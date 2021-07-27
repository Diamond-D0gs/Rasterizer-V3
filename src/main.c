#include <stdio.h>
#include <stdbool.h>
#include "typedefs.h"
#include "libbmp.h"

const int WIDTH  = 1024;
const int HEIGHT = 1024;

float EdgeFunction(Vec2i A, Vec2i B, Vec2i P);
void BoundingBox2D(const Vec2i triangle[], Vec2i* bboxmin, Vec2i* bboxmax);
Vec3f Barycentric2D(Vec2i triangle[], const float area, Vec2i P);
void RasterizeTriangle(bmp_img* img, Vertice verts[], int index[], int indexLoc);

int main(void) {
    Vertice verts[4] = {{{12, 12, 0}, {255, 0, 0}, {0, 0}},
                        {{12, 1012, 0}, {0, 255, 0}, {1, 0}},
                        {{1012, 1012, 0}, {0, 0, 255}, {0, 1}},
                        {{1012, 12, 0}, {255, 255, 255}, {1, 1}}};
    int index[6] = {0, 1, 2, 1, 3, 2};

    bmp_img img;
    bmp_img_init_df (&img, WIDTH, HEIGHT);

    for (int i = 0; i < 6; i += 3) {
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

void BoundingBox2D(const Vec2i triangle[], Vec2i* bboxmin, Vec2i* bboxmax) {
    *bboxmin = triangle[0];
    *bboxmax = triangle[0];

    for (int i = 1; i < 3; ++i) {
        if (triangle[i].x < bboxmin->x)
            bboxmin->x = triangle[i].x;
        else if (triangle[i].x > bboxmax->x)
            bboxmax->x = triangle[i].x;

        if (triangle[i].y < bboxmin->y)
            bboxmin->y = triangle[i].y;
        else if (triangle[i].y > bboxmax->y)
            bboxmax->y = triangle[i].y;
    }
}

Vec3f Barycentric2D(Vec2i triangle[], const float area, Vec2i P) {
    Vec3f bary;

    // ABP
    bary.x = EdgeFunction(triangle[0], triangle[1], P) / area;
    // BCP
    bary.y = EdgeFunction(triangle[1], triangle[2], P) / area;
    // CAP
    bary.z = EdgeFunction(triangle[2], triangle[0], P) / area;

    return bary;
}

void RasterizeTriangle(bmp_img* img, Vertice verts[], int index[], int indexLoc) {
    Vec2i triangle[3];
    triangle[0] = (Vec2i){verts[indexLoc].pos.x, verts[indexLoc].pos.y};
    triangle[1] = (Vec2i){verts[indexLoc+1].pos.x, verts[indexLoc+1].pos.y};
    triangle[2] = (Vec2i){verts[indexLoc+2].pos.x, verts[indexLoc+2].pos.y};
    
    float area = EdgeFunction(triangle[0], triangle[1], triangle[2]);
    if (area > 0) {
        Vec2i bboxmin, bboxmax;
        BoundingBox2D(triangle, &bboxmin, &bboxmax);

        printf("%i, %i, %i, %i\n", bboxmin.x, bboxmin.y, bboxmax.x, bboxmax.y);

        // Rasterize triangle within its bounding box
        for (int y = bboxmin.y; y <= bboxmax.y; ++y) {
            for (int x = bboxmin.x; x <= bboxmax.x; ++x) {
                Vec3f bary = Barycentric2D(triangle, area, (Vec2i){x, y});
                if (bary.u >= 0 && bary.v >= 0 && bary.w >= 0) {
                    RGB color;
					color.r = (bary.u * verts[indexLoc].color.r) + (bary.v * verts[indexLoc+1].color.r) + (bary.w * verts[indexLoc+2].color.r);
					color.g = (bary.u * verts[indexLoc].color.g) + (bary.v * verts[indexLoc+1].color.g) + (bary.w * verts[indexLoc+2].color.g);
					color.b = (bary.u * verts[indexLoc].color.b) + (bary.v * verts[indexLoc+1].color.b) + (bary.w * verts[indexLoc+2].color.b);
					bmp_pixel_init (&img->img_pixels[y][x], color.r, color.g, color.b);
                }
            }
        }
    }
    else {
        printf("piss\n");
    }
}