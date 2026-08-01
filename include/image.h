#pragma once
#include <vector>

class Executor;

class Image {
public:
  Image();
  Image(int w, int h, int c, std::vector<unsigned char> p);
  ~Image();

  int width, height, channels;
  std::vector<unsigned char> pixels;
};
