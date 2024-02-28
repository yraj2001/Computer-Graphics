#ifndef RD_ENGINE_DIRECT_H
#define RD_ENGINE_DIRECT_H

#include "rd_error.h"
#include "rd_direct.h"
#include "rd_enginebase.h"
#include "rd_display.h"

#include <string>
using std::string;

struct Point {
    double x;
    double y;
    double z;
};

struct PointH {
    double x;
    double y;
    double z;
    double w; 
};

struct Vector {
    double x;
    double y;
    double z;
};

struct XForm {
    double matrix[4][4];
};


extern int current_frame_no;
extern int current_xsize;
extern int current_ysize;


extern float currentColor[3];
extern float backgroundColor[3];
// This is a rendering engine that renders directly to the image buffer as
// primitives come in.  A depth buffer is obviously needed.  Transparency is
// not properly handled.


class REDirect: public RenderEngine
{
 public: int rd_display(const string & name, const string & type, 
			 const string & mode);
  public: int rd_format(int xresolution, int yresolution);
  public: int rd_frame_begin(int frame_no);
  public: int rd_world_begin(void);
  public: int rd_world_end(void);
  public: int rd_frame_end(void);
  public: int rd_color(const float color[]);
  public: int rd_background(const float color[]);
  public: int rd_point(const float p[3]);
  public: int rd_line(const float start[3], const float end[3]);
  public: int rd_circle(const float center[3], float radius);
  public: int rd_fill(const float seed_point[3]);
  // Only methods inherited from the RenderEngine class should be added here,
  // as needed.
};

#endif /* RD_ENGINE_DIRECT_H */
