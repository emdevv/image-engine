#pragma once
#include <vector>

class Executor;

class Image {
public:
  Image();
  Image(int w, int h, int c, std::vector<unsigned char> p);
  ~Image();

  // void convolution(std::vector<std::vector<float>> kernel);

  int width, height, channels;
  std::vector<unsigned char> pixels;
};
