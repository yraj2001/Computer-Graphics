#include "graphics_util.h"
#include "rd_error.h"
#include <cmath>
#include <stack>
#include <vector>
#include <iostream>

//stack definition
std::stack<xform> transformStack;

// Point and vector3d Operations
point multiply(float scalar, const point& p) {
    return {scalar * p.v[0], scalar * p.v[1], scalar * p.v[2]};
}

// By reusing the last function
point multiply(const point& p, float scalar) {
    return multiply(scalar, p); 
}

vector3d multiply(float scalar, const vector3d& v) {
    return {scalar * v.v[0], scalar * v.v[1], scalar * v.v[2]};
}

// By reusing the last function
vector3d multiply(const vector3d& v, float scalar) {
    return multiply(scalar, v);
}

void copy(point& dest, const point& src) {
    dest.v[0] = src.v[0];
    dest.v[1] = src.v[1];
    dest.v[2] = src.v[2];
}

void copy(vector3d& dest, const vector3d& src) {
    dest.v[0] = src.v[0];
    dest.v[1] = src.v[1];
    dest.v[2] = src.v[2];
}

double dot(const vector3d& v1, const vector3d& v2) {
    double r = v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2];
    return r;
}

vector3d cross(const vector3d& v1, const vector3d& v2) {
    vector3d r= {v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1],
            v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2],
            v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0]};
    return r;
}

//by using dot product function
double mag2(const vector3d& v) {
    return dot(v, v);
}

//it's better to save the original vector3d, we can use it later
vector3d normalize(const vector3d& v) {
    double mag = std::sqrt(mag2(v));
    return {v.v[0] / mag, v.v[1] / mag, v.v[2] / mag};
}

vector3d add(const vector3d& v1, const vector3d& v2) {
    return {v1.v[0] + v2.v[0], v1.v[1] + v2.v[1], v1.v[2] + v2.v[2]};
}

vector3d subtract(const vector3d& v1, const vector3d& v2) {
    return {v1.v[0] - v2.v[0], v1.v[1] - v2.v[1], v1.v[2] - v2.v[2]};
}

//using last function
vector3d subtract(const point& p1, const point& p2) {
    const vector3d p_a = {p1.v[0], p1.v[1], p1.v[2]};
    const vector3d p_b = {p2.v[0], p2.v[1], p2.v[2]};
    return subtract(p_a, p_b);
}

pointh convert(float p[3]){
    return {p[0], p[1], p[2], 1.0};
}
pointh convert(const point& p) {
    return {p.v[0], p.v[1], p.v[2], 1.0};
}
point convert(pointh p){
    point r;
    r.v[0] = p.v[0]/p.v[3];
    r.v[1] = p.v[1]/p.v[3];
    r.v[2] = p.v[2]/p.v[3];
    return r;
}
pointh convert(const vector3d& v) {
    return {v.v[0], v.v[1], v.v[2], 1.0};
}

// Transformations
xform identity() {
    xform m;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m.matrix[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    return m;
}

void identity(xform& m1) {
    m1 = identity();
}

xform multiply(const xform& m1, const xform& m2) {
    xform result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.matrix[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                result.matrix[i][j] +=m1.matrix[i][k] * m2.matrix[k][j];
            }
        }
    } 
    return result;
}

pointh multiply(const xform& m1, const pointh& p1) {
    pointh result;
    result.v[0] = (m1.matrix[0][0] * p1.v[0])+(m1.matrix[0][1] * p1.v[1])+(m1.matrix[0][2] * p1.v[2])+(m1.matrix[0][3] * p1.v[3]);
    result.v[1] = (m1.matrix[1][0] * p1.v[0])+(m1.matrix[1][1] * p1.v[1])+(m1.matrix[1][2] * p1.v[2])+(m1.matrix[1][3] * p1.v[3]);
    result.v[2] = (m1.matrix[2][0] * p1.v[0])+(m1.matrix[2][1] * p1.v[1])+(m1.matrix[2][2] * p1.v[2])+(m1.matrix[2][3] * p1.v[3]);
    result.v[3] = (m1.matrix[3][0] * p1.v[0])+(m1.matrix[3][1] * p1.v[1])+(m1.matrix[3][2] * p1.v[2])+(m1.matrix[3][3] * p1.v[3]);   
    // std::cout<<result.v[0]<<" "<<result.v[1]<<" "<<result.v[2]<<" multiply"<<std::endl; 
    return result;
}

void translate(xform& m1, double tx, double ty, double tz){
    xform t = identity();
    t.matrix[0][3] = tx;
    t.matrix[1][3] = ty;
    t.matrix[2][3] = tz;
    m1 = multiply(m1,t);
}

void scale(xform& m1, double sx, double sy, double sz){
    xform s = identity();
    s.matrix[0][0] = sx;
    s.matrix[1][1] = sy;
    s.matrix[2][2] = sz;
    m1 = multiply(m1,s);
}

//degree to radians
double con_d_r(double theta){
    return theta * M_PI / 180.0;
}
void rotate_xy(xform& m1, double theta) {
    double rad = con_d_r(theta);
    xform r = identity();
    r.matrix[0][0] = cos(rad);
    r.matrix[1][0] = sin(rad);
    r.matrix[0][1] = -sin(rad);
    r.matrix[1][1] = cos(rad);
    m1 = multiply(m1, r); // Apply the rotation on the right of m1
}

void rotate_yz(xform& m1, double theta) {
    double rad = con_d_r(theta);
    xform r = identity();
    r.matrix[1][1] = cos(rad);
    r.matrix[2][1] = sin(rad);
    r.matrix[1][2] = -sin(rad);
    r.matrix[2][2] = cos(rad);
    m1 = multiply(m1, r); 
}

void rotate_zx(xform& m1, double theta) {
    double rad = con_d_r(theta);
    xform r = identity();
    r.matrix[0][0] = cos(rad);
    r.matrix[0][2] = sin(rad);
    r.matrix[2][0] = -sin(rad);
    r.matrix[2][2] = cos(rad);
    m1 = multiply(m1, r);
}

xform world_to_camera(point eye, point at, vector3d up){
    vector3d A = subtract(at,eye);
    A = normalize(A);
    vector3d U = cross(A,up);
    U = normalize(U);
    vector3d V = cross(U,A);
    V = normalize(V); //not required
    xform r_inverse = identity();
    xform t_inverse = identity();
    //implementing r_inverse matrix
    r_inverse.matrix[0][0] = U.v[0];
    r_inverse.matrix[0][1] = U.v[1];
    r_inverse.matrix[0][2] = U.v[2];
    r_inverse.matrix[1][0] = V.v[0];
    r_inverse.matrix[1][1] = V.v[1];
    r_inverse.matrix[1][2] = V.v[2];
    r_inverse.matrix[2][0] = A.v[0];
    r_inverse.matrix[2][1] = A.v[1];
    r_inverse.matrix[2][2] = A.v[2];

    t_inverse.matrix[0][3] = -eye.v[0];
    t_inverse.matrix[1][3] = -eye.v[1];
    t_inverse.matrix[2][3] = -eye.v[2];

    return multiply(r_inverse,t_inverse);
}
// Transformation Stack Operations
void push(const xform& m1) {
    transformStack.push(m1);
}

xform pop() {
    if (!transformStack.empty()) {
        xform top = transformStack.top();
        transformStack.pop();
        return top;
    }
    return identity(); // Returns error statement for underflow
}

xform camera_to_clip(double fov, double near, double far, double aspect){
    double fov_r = con_d_r(fov/2.0);
    xform m = identity();
    
    //std::cout<<"tan theta/2 :  "<<tan(fov_r) <<std::endl;
    m.matrix[0][0] = 1/(2*(aspect*tan(fov_r)));
    m.matrix[0][2] = 1.0/2.0;
    m.matrix[1][1] = 1/(2*(tan(fov_r)));
    m.matrix[1][2] = 1.0/2.0;
    m.matrix[2][2] = far / (far- near);
    m.matrix[2][3] = (-far * near)/( far - near);
    m.matrix[3][2] = 1.0;
    m.matrix[3][3] = 0.0;
    return m;
}

xform clip_to_device(int width, int height){
    xform d = identity();
    d.matrix[0][0] = width;
    d.matrix[1][1] = -height;
    d.matrix[1][3] = height;
    return d;
}

