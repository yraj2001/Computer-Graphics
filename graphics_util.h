#ifndef GRAPHICS_UTIL_H
#define GRAPHICS_UTIL_H

#include "rd_error.h"
#include <cmath>
#include <stack>
#include <vector>
#include <iostream>

// Data Structures
struct point {
    double v[3];
};

struct pointh {
    double v[4];
};

struct vector3d {
    double v[3];
};

struct xform {
    double matrix[4][4];
};



// Global transformation stack
extern std::stack<xform> transformStack;

// Function Declarations

// Point and vector3d Operations
point multiply(float scalar, const point& p);
point multiply(const point& p, float scalar);
vector3d multiply(float scalar, const vector3d& v);
vector3d multiply(const vector3d& v, float scalar);

void copy(point& dest, const point& src);
void copy(vector3d& dest, const vector3d& src);

double dot(const vector3d& v1, const vector3d& v2);
vector3d cross(const vector3d& v1, const vector3d& v2);

double mag2(const vector3d& v);
vector3d normalize(const vector3d& v);
vector3d add(const vector3d& v1, const vector3d& v2);
vector3d subtract(const vector3d& v1, const vector3d& v2);
vector3d subtract(const point& p1, const point& p2);

pointh convert(float p[3]);
pointh convert(const point& p);
point convert(pointh p);
pointh convert(const vector3d& v);

// Transformations
xform identity();
void identity(xform& m1);

xform multiply(const xform& m1, const xform& m2);
pointh multiply(const xform& m1, const pointh& p1);

void translate(xform& m1, double tx, double ty, double tz);

void scale(xform& m1, double sx, double sy, double sz);

double con_d_r(double theta);
void rotate_xy(xform& m1, double theta);
void rotate_yz(xform& m1, double theta);
void rotate_zx(xform& m1, double theta);

xform world_to_camera(point eye, point at, vector3d up);

extern std::stack<xform> transformStack;
void push(const xform& m1);
xform pop();

xform camera_to_clip(double fov, double near, double far, double aspect);
xform clip_to_device(int width, int height);

#endif