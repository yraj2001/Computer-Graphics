#include "rd_direct.h"
#include "rd_error.h"
#include "rd_enginebase.h"
#include "rd_display.h"
#include "graphics_util.h"

int current_frame_no = 0;
int current_xsize = 640;
int current_ysize = 480;
double zbuffer[15000][15000]; //Initilizing at rd_world_begin

float current_color[3] = {1.0f,   1.0f,   1.0f}; // Default to white
float background_color[3] = {0.0f,   0.0f,   0.0f}; // Default to black

xform current_transform;
xform pipe_line;//from workd to clip

double current_fov = 90;
double current_near = 1.0;
double current_far = 1.0e+09;
double current_aspect = static_cast<double>(current_xsize)/static_cast<double>(current_ysize);

// default values are done in function REDirect::rd_world_begin
float eye_f[3] = {0,0,0};
float at_f[3] = {0,0,-1};
float up_f[3] ={0,1,0}; 
point eye,at;
vector3d up_v;

void plot_pixel(float x, float y, float z){
   // std::cout<<x<<" y:"<<y<<std::endl;
   // std::cout<<zbuffer[int(x)][int(y)]<<std::endl;
   if (zbuffer[int(x)][int(y)] > z){
      rd_write_pixel(int(x),int(y),current_color);
      zbuffer[int(x)][int(y)] = z;
   }
}

xform pipe_line_f(){
   xform x = identity();
   
      // object to world
    x = multiply(current_transform,x);
      // std::cout<<"Pipeline"<<std::endl;
      // std::cout<<x.matrix[0][0]<<" "<<x.matrix[0][1]<<" "<<x.matrix[0][2]<<" " <<x.matrix[0][3]<<" " <<std::endl;
      // std::cout<<x.matrix[1][0]<<" "<<x.matrix[1][1]<<" "<<x.matrix[1][2]<<" " <<x.matrix[1][3]<<" " <<std::endl;
      // std::cout<<x.matrix[2][0]<<" "<<x.matrix[2][1]<<" "<<x.matrix[2][2]<<" " <<x.matrix[2][3]<<" " <<std::endl;
      // std::cout<<x.matrix[3][0]<<" "<<x.matrix[3][1]<<" "<<x.matrix[3][2]<<" " <<x.matrix[3][3]<<" " <<std::endl;
       
    //world to camera
   eye.v[0] = eye_f[0]; eye.v[1] = eye_f[1]; eye.v[2] = eye_f[2];
   at.v[0] = at_f[0]; at.v[1] = at_f[1]; at.v[2] = at_f[2];
   up_v.v[0] = up_f[0]; up_v.v[1] = up_f[1]; up_v.v[2] = up_f[2];
   x = multiply(world_to_camera(eye,at,up_v),x);
     
      //camera to clip
   x = multiply(camera_to_clip(current_fov,current_near,current_far,current_aspect),x);
   
   return x;
}
void draw_line(const float start[3], const float end[3]){
   // std::cout<<"LINE"<<std::endl;
   // std::cout<<"Start :"<<start[0]<<" "<<start[1]<<" "<<start[2]<<std::endl;
   // std::cout<<"End :"<<end[0]<<" "<<end[1]<<" "<<end[2]<<std::endl;
   int dx = static_cast<int>(end[0]) - static_cast<int>(start[0]);
   int dy = static_cast<int>(end[1]) - static_cast<int>(start[1]);
   int nsteps = std::max(std::abs(dx),std::abs(dy));
   for(int i = 0;i<nsteps;i++){
      float x  = start[0] + static_cast<float>(i)/nsteps*(end[0]-start[0]);
      float y  = start[1] + static_cast<float>(i)/nsteps*(end[1]-start[1]);
      float z  = start[2] + static_cast<float>(i)/nsteps*(end[2]-start[2]);
      plot_pixel(x,y,z);
   }

}

void draw_line(point start, point end){
   const float s[3] = {static_cast<float>(start.v[0]),static_cast<float>(start.v[1]),static_cast<float>(start.v[2])};
   const float e[3] = {static_cast<float>(end.v[0]),static_cast<float>(end.v[1]),static_cast<float>(end.v[2])};
   draw_line(s,e);

}
void line_sequence(std::vector<pointh>& points){
   line_pipeline(points[0],0);
   // std::cout<<points[0].v[0]<<" "<<points[0].v[1]<<" "<<points[0].v[2]<<"   0"<<std::endl;
   for(int i = 1;i<points.size();i++){

   // std::cout<<points[i].v[0]<<" "<<points[i].v[1]<<" "<<points[i].v[2]<<"   1"<<std::endl;
      // std::cout<<"line"<<i<<std::endl;
      line_pipeline(points[i],1);
   }
}
pointh p0_c;
void line_pipeline(pointh p1, int flag){
   xform x;
   pointh p0_d,p1_d;
   
   // std::cout<<"Line : "<<p1.v[0]<<" "<<p1.v[1]<<" "<<p1.v[2]<<" "<<p1.v[3]<<" "<<std::endl;
   
   pointh p1_c = multiply(pipe_line,p1);
   if(flag != 0){
      //clipping
      int kode0 = 0,kode1 = 0;
      // std::cout<<p1_c.v[0]<<" "<<p1_c.v[1]<<" "<<p1_c.v[2]<<" "<<p1_c.v[3]<<" "<<std::endl;
      kode0 = kode_conversion(p0_c);
      kode1 = kode_conversion(p1_c);

      x =clip_to_device(current_xsize,current_ysize);

      if((kode0 & kode1)!= 0){ 
         // std::cout<<"REJECT"<<std::endl;
         // std::cout<<"Hello"<<std::endl;
         //trivial reject
         p0_c = p1_c;
         return;
      }
      if((kode0 | kode1) == 0){
         // std::cout<<"ACCEPT"<<std::endl;
         // std::cout<<"Hello"<<std::endl;
         // trivial accept
         p0_d = multiply(x,p0_c);
         p1_d = multiply(x,p1_c);
         draw_line(convert(p0_d),convert(p1_d));   
      }
      else{
         // std::cout<<"CLIP"<<std::endl;
         //part of this needs to be clipped
         pointh p0_c_,p1_c_;
         BC bc0 = BC_conversion(p0_c);
         BC bc1 = BC_conversion(p1_c);
         double alpha0 =0;
         double alpha1 =1;
         double alpha;
         int kode = kode0 | kode1;
         int mask = 1;


         for(int i = 0;i<6;i++,mask<<=1){
            // std::cout<<"i :"<<i<<" Mask :"<<std::bitset<6>(mask)<<std::endl;
            if(!(kode & mask))
               continue;
            alpha = bc0.bc[i]/(bc0.bc[i] - bc1.bc[i]);
            if(kode0 & mask){
               alpha0 = std::max(alpha0,alpha);
            }
            else{
               alpha1 = std::min(alpha1,alpha);
            }
            if(alpha1<alpha0){
               p0_c = p1_c;
               return;
            }
            // std::cout<<"Alpha 0 :"<<alpha0<<std::endl;
            // std::cout<<"Alpha 1 :"<<alpha1<<std::endl;
         }
         p0_c_ = line_clipping(p0_c,p1_c,alpha0);
         p1_c_ = line_clipping(p0_c,p1_c,alpha1);
         p0_d = multiply(x,p0_c_);
         p1_d = multiply(x,p1_c_);
         draw_line(convert(p0_d),convert(p1_d));

      }
   }
   p0_c = p1_c;
}
void point_pipeline(pointh p){
   
   //clip to device
   xform x = pipe_line;
   pointh ploth = multiply(x,p);

   // std::cout<<ploth.v[0]<<" "<<ploth.v[1]<<" "<<ploth.v[2]<<" "<<ploth.v[3]<<std::endl;
   // std::cout<<(ploth.v[3]-ploth.v[0])<<" "<<(ploth.v[3]-ploth.v[1])<<" "<<(ploth.v[3]-ploth.v[2])<<std::endl;
   
   //x,w-x,y,w-y,z,w-z
   if(ploth.v[0]>=0.0 && (ploth.v[3]-ploth.v[0])>=0.0 && ploth.v[1]>=0.0 && (ploth.v[3]-ploth.v[1])>=0.0 && ploth.v[2]>=0.0 && (ploth.v[3]-ploth.v[2])>=0.0){
   ploth = multiply(clip_to_device(current_xsize,current_ysize),ploth);
   point plot = convert(ploth);
   // std::cout<<plot.v[0]<<" "<<plot.v[1]<<" "<<plot.v[2]<<" "<<std::endl;
   plot_pixel(plot.v[0],plot.v[1],plot.v[2]);
   }
   // std::cout<<std::endl;
}

void circle_3d(float radius){
   pointh p1,p2;
   p1.v[0] = radius;   p1.v[1] = 0; p1.v[2] = 0; p1.v[3] = 1;
   p2.v[2] = 0;   p2.v[3] = 1;
   double theta;
   line_pipeline(p1,0);
   for(int i = 1;i<=36;i++){
      theta = (i/36.0)  * 360;
      // std::cout<<i<<" "<<theta<<std::endl;
      p2.v[0] = radius * cos(con_d_r(theta));
      p2.v[1] = radius * sin(con_d_r(theta));
      // std::cout<<i<<" "<<p2.v[0]<<" "<<p2.v[1]<<" "<<std::endl;
      line_pipeline(p2,1);
      p1 = p2;
   }
}

int REDirect::rd_display(const string & name, const string & type, 
			 const string & mode){
                return RD_OK;
             }
int REDirect::rd_format(int xresolution, int yresolution){
   current_xsize = xresolution;
   current_ysize = yresolution;
   current_aspect = static_cast<double>(xresolution)/static_cast<double>(yresolution);
   return RD_OK;
}
int REDirect::rd_frame_begin(int frame_no){
   current_frame_no = frame_no;
   return RD_OK;
}

int REDirect::rd_camera_eye(const float eyepoint[3]){
   eye_f[0] = eyepoint[0];
   eye_f[1] = eyepoint[1];
   eye_f[2] = eyepoint[2];
   return RD_OK;
}
int REDirect::rd_camera_at(const float atpoint[3]){
   at_f[0] = atpoint[0];
   at_f[1] = atpoint[1];
   at_f[2] = atpoint[2];
   return RD_OK;
}
int REDirect::rd_camera_up(const float up[3]){
   up_f[0] = up[0];
   up_f[1] = up[1];
   up_f[2] = up[2];
   return RD_OK;
}
int REDirect::rd_camera_fov(float fov){
   current_fov = fov;
   return RD_OK;
}
int REDirect::rd_clipping(float znear, float zfar){
   current_near = znear;
   current_far = zfar;
   return RD_OK;
}

int REDirect::rd_world_begin(void){

   current_transform = identity();
   pipe_line = pipe_line_f();

   for(int i = 0;i<current_xsize;i++){
      for(int j = 0;j<current_ysize;j++){
         zbuffer[i][j] = 1;
      }
   } 

   rd_disp_init_frame(current_frame_no);
   return RD_OK;
}
int REDirect::rd_world_end(void){
   rd_disp_end_frame();
   return RD_OK;
}
int REDirect::rd_frame_end(void){
   rd_disp_end_frame();
   return RD_OK;
}
int REDirect::rd_color(const float color[]){
   current_color[0] = color[0];
   current_color[1] = color[1];
   current_color[2] = color[2];
   return RD_OK;
}
int REDirect::rd_background(const float color[]){
   background_color[0] = color[0];
   background_color[1] = color[1];
   background_color[2] = color[2];
   rd_set_background(background_color);
   return RD_OK;
}

int REDirect::rd_point(const float p[3]){
   pointh point;
   point.v[0] = p[0];
   point.v[1] = p[1];
   point.v[2] = p[2];
   point.v[3] = 1;
   point_pipeline(point);
   return RD_OK;
}
int REDirect::rd_line(const float start[3], const float end[3]){
   pointh start_ph,end_ph;
   start_ph.v[0] = start[0];
   start_ph.v[1] = start[1];
   start_ph.v[2] = start[2];
   start_ph.v[3] = 1;
   end_ph.v[0] = end[0];
   end_ph.v[1] = end[1];
   end_ph.v[2] = end[2];
   end_ph.v[3] = 1;
   line_pipeline(start_ph,0);
   line_pipeline(end_ph,1);
   return RD_OK;
}
int REDirect::rd_circle(const float center[3], float radius){
int center_x = static_cast<int>(center[0]);
int center_y = static_cast<int>(center[1]);
int r = radius;

int x = 0;
int y = r;
int p = 1 - r;
while(y >= x){

   rd_write_pixel(x+center_x,y+center_y,current_color);
   rd_write_pixel(y+center_x,x+center_y,current_color);
   rd_write_pixel(-x+center_x,y+center_y,current_color);
   rd_write_pixel(-y+center_x,x+center_y,current_color);
   rd_write_pixel(-y+center_x,-x+center_y,current_color);
   rd_write_pixel(-x+center_x,-y+center_y,current_color);
   rd_write_pixel(x+center_x,-y+center_y,current_color);
   rd_write_pixel(y+center_x,-x+center_y,current_color);

   x++;
   if(p>=0){
      y--;
      p = p + 2*x + 1 -(2*y); 
   }
   else
   p = p + 2*x + 1;
}
   return RD_OK;
}
int REDirect::rd_fill(const float seed_point[3]){
   int seed_x = static_cast<int>(seed_point[0]);
   int seed_y = static_cast<int>(seed_point[1]);
   if (seed_x < 0 || seed_x > current_xsize || seed_y < 0 || seed_y > current_ysize )
      return RD_OK;
   float seed_color[3];
   rd_read_pixel(seed_x, seed_y, seed_color);
   if (seed_color[0] == current_color[0] && seed_color[1] == current_color[1] && seed_color[2] == current_color[2]) {
      return RD_OK;
   }
   int start = -1;
   int end = -1;
   int flag =  0;
   int i = seed_x;
   int j = i;

   while(start <  0){
      float read_color[3];
      if (flag ==  0){
         if(j<current_xsize){
            j++;
            rd_read_pixel(j, seed_y, read_color);
            if(seed_color[0] != read_color[0] || seed_color[1] != read_color[1] || seed_color[2] != read_color[2]){
               end = --j;
               flag =  1;
            }
         }
         else{
            end = current_xsize - 1;
            flag = 1;
         }
      }
      else{
         if(i>=0){
         i--;
         rd_read_pixel(i, seed_y, read_color);
         if(seed_color[0] != read_color[0] || seed_color[1] != read_color[1] || seed_color[2] != read_color[2]){
            start = ++i;
         }
         }
         else
            start = 0;
      }
      // std::cout << "loop done"<<std::endl;
   }

   // std::cout << "start : " << start << " End : " << end << " y : " << seed_y << std::endl;
   for(int i = start; i <= end; i++){
      rd_write_pixel(i, seed_y, current_color);
   }

   for(int i = start; i <= end; i++){
      if((seed_y -  1) < 0)
         return RD_OK;
      float up_color[3];
      float down_color[3];
      rd_read_pixel(i, seed_y -  1, up_color);
      if(seed_color[0] == up_color[0] && seed_color[1] == up_color[1] && seed_color[2] == up_color[2]){
         float up[3] = {static_cast<float>(i), static_cast<float>(seed_y -  1),  0.0f};
         rd_fill(up);
      }
      if ((seed_y +  1) > current_ysize)
         return RD_OK;
      rd_read_pixel(i, seed_y +  1, down_color);
      if(seed_color[0] == down_color[0] && seed_color[1] == down_color[1] && seed_color[2] == down_color[2]){
         float down[3] = {static_cast<float>(i), static_cast<float>(seed_y +  1),  0.0f};
         rd_fill(down);
      }
   }

   return RD_OK;
}

int REDirect::rd_translate(const float offset[3]){
   translate(current_transform, offset[0],offset[1],offset[2]);
   pipe_line = pipe_line_f();
   return RD_OK;
}
int REDirect::rd_scale(const float scale_factor[3]){
   // std::cout<<scale_factor[0]<<" "<<scale_factor[0]<<" "<<scale_factor[0]<<" "<<std::endl;
   scale(current_transform,scale_factor[0],scale_factor[1],scale_factor[2]);
   pipe_line = pipe_line_f();
   return RD_OK;
}
int REDirect::rd_rotate_xy(float angle){
   rotate_xy(current_transform,angle);
   pipe_line = pipe_line_f();
   return RD_OK;
}
int REDirect::rd_rotate_yz(float angle){
   rotate_yz(current_transform,angle);
   pipe_line = pipe_line_f();
   return RD_OK;
}
int REDirect::rd_rotate_zx(float angle){
   rotate_zx(current_transform,angle);
   pipe_line = pipe_line_f();
   return RD_OK;
}

int REDirect:: rd_pointset(const string & vertex_type,int nvertex, const vector<float> & vertex){
   for (int i = 0; i<nvertex;i++){
      float p[3] = {vertex[i*3],vertex[i*3+1],vertex[i*3+2]};
      rd_point(p);
   }
   return RD_OK;
}
int REDirect:: rd_cube(void){
   pointh v[8];
   v[0] = { 1.0,-1.0, 1.0, 1.0};
   v[1] = { 1.0, 1.0, 1.0, 1.0};
   v[2] = { 1.0, 1.0,-1.0, 1.0};
   v[3] = { 1.0,-1.0,-1.0, 1.0};
   v[4] = {-1.0,-1.0, 1.0, 1.0};
   v[5] = {-1.0, 1.0, 1.0, 1.0};
   v[6] = {-1.0, 1.0,-1.0, 1.0};
   v[7] = {-1.0,-1.0,-1.0, 1.0};

   std::vector<pointh> points = {v[1],v[0],v[3],v[2],v[1]};
    line_sequence(points);
   points = {v[1],v[5],v[4],v[0],v[1]};
   line_sequence(points);
   points = {v[2],v[6],v[5],v[1],v[2]};
   line_sequence(points);
   points = {v[4],v[5],v[6],v[7],v[4]};
   line_sequence(points);
   points = {v[0],v[4],v[7],v[3],v[0]};
   line_sequence(points);
   points = {v[7],v[6],v[2],v[3],v[7]};
   line_sequence(points);
   return RD_OK;
}
int REDirect:: rd_sphere(float radius, float zmin, float zmax, float thetamax){
   sphere(radius);
   return RD_OK;
}
pointh sphere_points(float r,double phe, double theta){
   // std::cout<<"called"<<std::endl;
   pointh p;
   p.v[0] = r * cos(con_d_r(phe)) * cos(con_d_r(theta));
   p.v[1] = r * cos(con_d_r(phe)) * cos(con_d_r(theta));
   p.v[2] = r * sin(con_d_r(phe));
   p.v[3] = 1;
   return p; 
}
void sphere(float radius){
   // std::cout<<"called"<<std::endl;
   float n1steps = 24;
   float n2steps = 48;
   double theta,phe,theta2,phe2;
   for(int i = 0;i<n1steps;i++){
      phe = static_cast<double>(((i/n1steps) * 180.0) - 90.0);
      phe2 = static_cast<double>((((i+1)/n1steps) * 180.0) - 90.0);
      for(int j = 0;j<=n2steps;j++){
         theta = (j/n2steps) * 360.0;
         theta2 = ((j+1)/n2steps) * 360.0;

         // std::cout<<"phe :"<<phe<<" theta :"<<theta<<std::endl;
         pointh p1,p2,p3,p4;
         p1 = sphere_points(radius,phe,theta);
         p2 = sphere_points(radius,phe,theta2);
         p3 = sphere_points(radius,phe2,theta2);
         p4 = sphere_points(radius,phe2,theta);
         // std::cout<<p1.v[0]<<" "<<p1.v[1]<<" "<<p1.v[2]<<" "<<p1.v[3]<<std::endl;
         line_pipeline(p1,0);
         line_pipeline(p2,1);
         line_pipeline(p3,1);
         line_pipeline(p4,1);
         line_pipeline(p1,1);
      }
   } 
}
int REDirect:: rd_cone(float height, float radius, float thetamax){
   pointh p1,h;
   p1.v[0] = radius;   p1.v[1] = 0; p1.v[2] = 0;p1.v[3] = 1;
   h.v[0] = 0; h.v[1] = 0; h.v[2] = height; h.v[3] = 1;

   line_pipeline(p1,0);
   line_pipeline(h,1);
   line_pipeline(p1,0);

   double theta;
   for(int i = 1;i<=36;i++){
      theta = (i/36.0)  * 360;
      p1.v[0] = radius * cos(con_d_r(theta));
      p1.v[1] = radius * sin(con_d_r(theta));
      line_pipeline(p1,1);
      line_pipeline(h,1);
      line_pipeline(p1,0);

   }
   return RD_OK;
}
int REDirect:: rd_cylinder(float radius, float zmin, float zmax, float thetamax){
   pointh p1,p2,p3,p4;
   p1.v[0] = radius;   p1.v[1] = 0; p1.v[2] = zmin; p1.v[3] = 1;
   p2.v[2] = zmin; p2.v[3] = 1;
   p3.v[0] = radius;   p3.v[1] = 0; p3.v[2] = zmax; p3.v[3] = 1;
   p4.v[2] = zmax; p4.v[3] = 1;
   
   double theta;
   for(int i = 1;i<=36;i++){
      theta = (i/36.0) * 2 * 360;
      p2.v[0] = radius * cos(con_d_r(theta));
      p2.v[1] = radius * sin(con_d_r(theta));

      p4.v[0] = radius * cos(con_d_r(theta));
      p4.v[1] = radius * sin(con_d_r(theta));
      line_pipeline(p1,0);
      line_pipeline(p2,1);
      line_pipeline(p3,0);
      line_pipeline(p4,1);
      line_pipeline(p2,0);
      line_pipeline(p4,1);
      p1 = p2;
      p3 = p4;
   }
   return RD_OK;
}
int REDirect:: rd_disk(float height, float radius, float theta){
   pointh p;
   p.v[0] = radius;   p.v[1] = 0; p.v[2] = height; p.v[3] = 1;
   line_pipeline(p,0);
   double t;
   for(int i = 1;i<36;i++){
      t = (i/36.0) * 360;
      p.v[0] = radius * cos(con_d_r(t));
      p.v[1] = radius * sin(con_d_r(t));
      line_pipeline(p,1);
   }
   return RD_OK;
}
int REDirect:: rd_polyset(const string & vertex_type, int nvertex, const vector<float> & vertex,
			 int nface,   const vector<int> & face){
         std::vector<pointh> vertices(nvertex);
         for(int i = 0; i<nvertex;i++){
            vertices[i].v[0] = vertex[(i*3)];
            vertices[i].v[1] = vertex[(i*3) + 1];
            vertices[i].v[2] = vertex[(i*3) + 2];
            vertices[i].v[3] = 1;
            // std::cout<<vertices[i].v[0]<<" "<<vertices[i].v[1]<<" "<<vertices[i].v[2]<<" "<<std::endl;
         }
         int i = 0;
         int j = 0;
         while( i<nface){
            int k = face[j];
            line_pipeline(vertices[face[j]],0);
            j++;
            while(face[j] != -1){
               line_pipeline(vertices[face[j]],1);
               j++;
            }
            line_pipeline(vertices[k],1);
            j++;
            i ++;
         }
         return RD_OK;
}

  int REDirect:: rd_xform_push(void){
   push(current_transform);
       
   return RD_OK;
  }
  int REDirect:: rd_xform_pop(void){
   current_transform = pop();
   pipe_line = pipe_line_f();
    
   return RD_OK;
  }


