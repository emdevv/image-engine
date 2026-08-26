#pragma once

#include <sycl/sycl.hpp>
#include "image.h"
#include "sycl/queue.hpp"

class CropOp {
public:
  int crop_x;
  int crop_y;
  int crop_w;
  int crop_h;

  CropOp(int x, int y, int w, int h) : crop_x(x), crop_y(y), crop_w(w), crop_h(h) {}

  void apply_native(Image &img, Image &img_out);
  void apply_kernel(Image &img, Image &img_out, sycl::queue &q);
};

class BlurOp {
public:
  int percentage;

  BlurOp(int p) : percentage(p) {}

  void apply_native(Image &img, Image &img_out);
  void apply_kernel(Image &img, Image &img_out, sycl::queue &q);
};

class RotateOp {
public:
  int angle;

  RotateOp(int a) : angle(a) {}

  void apply_native(Image &img_in, Image &img_out);
  void apply_kernel(Image &img_in, Image &img_out, sycl::queue &q);
};

class ConvolutionOp {
public:
  std::vector<std::vector<float>> kernel;
  std::vector<float> flat_kernel;
  int kw;
  int kh;
  int halfH;
  int halfW;

  ConvolutionOp(std::vector<std::vector<float>> k) : kernel(k) {
    kh = kernel.size();
    kw = kernel[0].size();
    halfH = kh / 2;
    halfW = kw / 2;

    flat_kernel.resize(kh * kw);
    for (int y = 0; y < kh; ++y) {
      for (int x = 0; x < kw; ++x) {
        flat_kernel[y * kw + x] = kernel[y][x];
      }
    }
  }

  void apply_native(Image &img, Image &img_out);
  void apply_kernel(Image &img, Image &img_out, sycl::queue &q);
};
