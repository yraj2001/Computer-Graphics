#include "rd_error.h"
#include "rd_direct.h"
#include "rd_enginebase.h"
#include "rd_display.h"

#include <cmath>
#include <iostream>

int current_frame_no = 0;
int current_xsize = 640;
int current_ysize = 480;

float current_color[3] = {1.0f,   1.0f,   1.0f}; // Default to white
float background_color[3] = {0.0f,   0.0f,   0.0f}; // Default to black

int REDirect::rd_display(const string & name, const string & type, 
			 const string & mode){
                return RD_OK;
             }
int REDirect::rd_format(int xresolution, int yresolution){
   current_xsize = xresolution;
   current_ysize = yresolution; 
   return RD_OK;
}
int REDirect::rd_frame_begin(int frame_no){
   current_frame_no = frame_no;
   return RD_OK;
}
int REDirect::rd_world_begin(void){

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
   rd_write_pixel(p[0], p[1], current_color);
   return RD_OK;
}
int REDirect::rd_line(const float start[3], const float end[3]){
   int x1 = static_cast<int>(start[0]);
   int y1 = static_cast<int>(start[1]);
   int x2 = static_cast<int>(end[0]);
   int y2 = static_cast<int>(end[1]);

   if(std::abs(x2-x1) > std::abs(y2-y1)){
      if (x1 > x2){
         int x3 = x1;
         x1 = x2;
         x2 = x3;
         int y3 = y1;
         y1 = y2;
         y2 = y3;
      }
      int delta_x = x2 - x1;
      int delta_y = y2 - y1;

      int p = 2*delta_y - delta_x;
      int y = y1;
      int x = x1;
      
      if (delta_y>=0){
      for (; x<=x2;x++){
         rd_write_pixel(x,y,current_color);
         if (p >= 0){
            p = p + (2*delta_y - 2*delta_x);
            y++;
         }
         else{
            p = p + (2*delta_y);
         }
      }
      }
      else{
         // float trail[3] = {1.0f,1.0f,1.0f};
         // rd_write_pixel(400,400,trail);
         
         for (; x<=x2;x++){

            rd_write_pixel(x,y,current_color);
            if (p >= 0){
               p = p - (2*delta_y + 2*delta_x);
               y--;
            }
            else{
               p = p - (2*delta_y);
               //y--;
            }
         }
      }
      
   }
   else{
      if (y1 > y2){
         int x3 = x1;
         x1 = x2;
         x2 = x3;
         int y3 = y1;
         y1 = y2;
         y2 = y3;
      }
      int delta_x = x2 - x1;
      int delta_y = y2 - y1;

      int p = 2*delta_x - delta_y;
      int y = y1;
      int x = x1;
      if (delta_x>=0){
      for (; y<=y2;y++){
         rd_write_pixel(x,y,current_color);
         if (p >= 0){
            p = p + (2*delta_x - 2*delta_y);
            x++;
         }
         else{
            p = p + (2*delta_x);
         }
      }
      }
      else{
         // float trail[3] = {1.0f,1.0f,1.0f};
         // rd_write_pixel(400,400,trail);
         p = -2*delta_x - delta_y;
         // std::cout<<x<<" "<<y<<std::endl;
         // std::cout<<x2<<" "<<delta_x<<" "<<p<<" "<<delta_y<<std::endl;
         for (; y<=y2;y++){
            rd_write_pixel(x,y,current_color);
            if (p >= 0){
               p = p - (2*delta_x) - (2*delta_y);
               x--;
            }
            else{
               p = p - (2*delta_x);
            }
         }
      }
   }
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

