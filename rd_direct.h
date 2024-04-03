#ifndef RD_ENGINE_DIRECT_H
#define RD_ENGINE_DIRECT_H

#include "rd_error.h"
#include "rd_enginebase.h"
#include "rd_display.h"
#include "graphics_util.h"

#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
using std::string;

extern int current_frame_no;
extern int current_xsize;
extern int current_ysize;


extern float currentColor[3];
extern float backgroundColor[3];
// This is a rendering engine that renders directly to the image buffer as
// primitives come in.  A depth buffer is obviously needed.  Transparency is
// not properly handled.
xform pipe_line_f();
 void draw_line(const float start[3], const float end[3]);
 void draw_line(point start, point end);
 void line_pipeline(pointh p1, int flag);
 void point_pipeline(pointh p);
 void circle_3d(float radius);

class REDirect: public RenderEngine
{
  
 public: int rd_display(const string & name, const string & type, 
			 const string & mode);
  public: int rd_format(int xresolution, int yresolution);
  public: int rd_frame_begin(int frame_no);

  public: int rd_camera_eye(const float eyepoint[3]);
  public: int rd_camera_at(const float atpoint[3]);
  public: int rd_camera_up(const float up[3]);
  public: int rd_camera_fov(float fov);
  public: int rd_clipping(float znear, float zfar);

  public: int rd_world_begin(void);
  public: int rd_world_end(void);
  public: int rd_frame_end(void);
  public: int rd_color(const float color[]);
  public: int rd_background(const float color[]);

  public: int rd_point(const float p[3]);
  public: int rd_line(const float start[3], const float end[3]);
  public: int rd_circle(const float center[3], float radius);

  public: int rd_fill(const float seed_point[3]);
  public: int rd_translate(const float offset[3]);
  public: int rd_scale(const float scale_factor[3]);
  public: int rd_rotate_xy(float angle);
  public: int rd_rotate_yz(float angle);
  public: int rd_rotate_zx(float angle);

  public: int rd_pointset(const string & vertex_type,
			  int nvertex, const vector<float> & vertex);

  public: int rd_cube(void);
  public: int rd_sphere(float radius, float zmin, float zmax, float thetamax);
  public: int rd_cone(float height, float radius, float thetamax);
  public: int rd_cylinder(float radius, float zmin, 
			  float zmax, float thetamax);
  public: int rd_disk(float height, float radius, float theta);
  public: int rd_polyset(const string & vertex_type, 
			 int nvertex, const vector<float> & vertex,
			 int nface,   const vector<int> & face);

  public: int rd_xform_push(void);
  public: int rd_xform_pop(void);
  // Only methods inherited from the RenderEngine class should be added here,
  // as needed.
};

#endif /* RD_ENGINE_DIRECT_H */
