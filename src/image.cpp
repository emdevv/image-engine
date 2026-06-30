#include "image.h"
#include "ops.h"

Image::Image() {}
Image::Image(int w, int h, int c, std::vector<unsigned char> p)
    : width(w), height(h), pixels(p) {}
Image::~Image() {}

void Image::crop(int crop_x, int crop_y, int crop_w, int crop_h) {
  CropOp::apply(*this, crop_x, crop_y, crop_w, crop_h);
}

void Image::blur(int blur) { BlurOp::apply(*this, blur); }

void Image::rotate(int angle) { RotateOp::apply(*this, angle); }