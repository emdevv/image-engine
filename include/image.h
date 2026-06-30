#pragma once
#include <vector>

class Image {
public:
  Image();
  Image(int w, int h, int c, std::vector<unsigned char> p);
  ~Image();

  void crop(int x, int y, int w, int h);
  void blur(int blur);
  void rotate(int angle);

  int width, height, channels;
  std::vector<unsigned char> pixels;
};