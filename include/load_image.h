#pragma once
#include <string>
#include <vector>

struct LoadedImage {
  int width = 0;
  int height = 0;
  int channels = 0;
  std::vector<unsigned char> pixels;
};

LoadedImage load_image(const std::string &filepath);
