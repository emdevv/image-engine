#pragma once

#include <sycl/sycl.hpp>

#include "image.h"

class CropOp {
public:
  static void apply(Image &img, int crop_x, int crop_y, int crop_w, int crop_h);
};

class BlurOp {
public:
  static void apply(Image &img, int blur);
};

class RotateOp {
public:
  int angle;

  RotateOp(int a) : angle(a) {}

  void apply_native(Image &img);
  void apply_kernel(Image &img, sycl::queue &q);
};

class ConvolutionOp {
public:
  static void apply(Image &img, std::vector<std::vector<float>> kernel);
};
