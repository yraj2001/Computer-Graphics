#include "rd_error.h"
#include "pnm_display.h"
#include "rd_direct.h"

#include <iostream>

unsigned char* image_data = nullptr;
int active_frame_no;
float current_background_color[3] = {0.0f, 0.0f, 0.0f};

int pnm_init_display(void){
  image_data = new unsigned char[display_xSize * display_ySize *  3];
  return RD_OK;
}

int pnm_end_display(void){
  delete[] image_data;
  return RD_OK;
}

int pnm_init_frame(int frame_no){
  active_frame_no = frame_no;
  pnm_clear();;
  //working
  return RD_OK;
}

int pnm_end_frame(void){
  std::ostringstream filenameStream;
  filenameStream << display_name << active_frame_no << ".ppm";
  std::string filename = filenameStream.str();

  std::ofstream ppmFile(filename, std::ios::binary);
  if (!ppmFile.is_open()) {
    // to check if file is created
    return RD_INPUT_FILE_NOT_FOUND;
  }
  // writing image data header and pixel values
  ppmFile << "P6\n" << display_xSize << " " << display_ySize << "\n255\n";
  ppmFile.write(reinterpret_cast<char*>(image_data), display_xSize * display_ySize *   3);

  ppmFile.close();

  return RD_OK;
}

int pnm_write_pixel(int x, int y, const float rgb []){
  //working need to be tested more
  image_data[(y * display_xSize + x) *  3] = static_cast<unsigned char>(rgb[0] *  255.0f);
  image_data[(y * display_xSize + x) *  3 +  1] = static_cast<unsigned char>(rgb[1] *  255.0f);
  image_data[(y * display_xSize + x) *  3 +  2] = static_cast<unsigned char>(rgb[2] *  255.0f);
  return RD_OK;
}

int pnm_read_pixel(int x, int y, float rgb []){
//working need to be tested more
  rgb[0] = image_data[(y * display_xSize + x) *  3] / 255.0f;
  rgb[1] = image_data[(y * display_xSize + x) *  3 + 1] / 255.0f;
  rgb[2] = image_data[(y * display_xSize + x) *  3 + 2] / 255.0f;
  return RD_OK;
}

int pnm_set_background(const float rgb []){
  current_background_color[0] = rgb[0];
  current_background_color[1] = rgb[1];
  current_background_color[2] = rgb[2];

  // std::cout << "R=" << static_cast<int>(current_background_color[0]) << ", ";
  // std::cout << "G=" << static_cast<int>(current_background_color[1]) << ", ";
  // std::cout << "B=" << static_cast<int>(current_background_color[2]) << std::endl;

  // for (int y =  0; y < display_ySize; ++y) {
  //   for (int x =  0; x < display_xSize; ++x) {
  //     int index = (y * display_xSize + x) *  3;
  //     std::cout << "Pixel (" << x << ", " << y << "): ";
  //     std::cout << "R=" << static_cast<int>(image_data[index]) << ", ";
  //     std::cout << "G=" << static_cast<int>(image_data[index +  1]) << ", ";
  //     std::cout << "B=" << static_cast<int>(image_data[index +  2]) << std::endl;
  //   }
  // }
  return RD_OK;
}

int pnm_clear(void){
  //   std::cout << "R=" << static_cast<int>(current_background_color[0]) << ", ";
  // std::cout << "G=" << static_cast<int>(current_background_color[1]) << ", ";
  // std::cout << "B=" << static_cast<int>(current_background_color[2]) << std::endl;
  if (image_data == nullptr)
    return RD_INPUT_DISPLAY_INITIALIZATION_ERROR;

  for(int y = 0; y < display_ySize; y++){
    for(int x = 0; x < display_xSize; x++){
      int location = (y*display_xSize+x)*3;
      image_data[location] = static_cast<unsigned char>(current_background_color[0] *  255.0f);
      image_data[location +  1] = static_cast<unsigned char>(current_background_color[1] *  255.0f);
      image_data[location +  2] = static_cast<unsigned char>(current_background_color[2] *  255.0f);
    }
    int firstPixelLocation =  0;
    if (image_data[firstPixelLocation] != static_cast<unsigned char>(current_background_color[0] *   255.0f) ||
      image_data[firstPixelLocation +   1] != static_cast<unsigned char>(current_background_color[1] *   255.0f) ||
      image_data[firstPixelLocation +   2] != static_cast<unsigned char>(current_background_color[2] *   255.0f)) {
      return RD_INPUT_DISPLAY_INITIALIZATION_ERROR;
    }
  }
  return RD_OK;
}