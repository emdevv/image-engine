#include "image.h"
#include "crop.h"

Image::Image() {}
Image::Image(int w, int h, int c, std::vector<unsigned char> p)
    : width(w), height(h), pixels(p) {}
Image::~Image() {}

void Image::crop(int x, int y, int w, int h) { CropOp::apply(*this); }