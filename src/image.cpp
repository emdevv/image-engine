#include "image.h"
#include <vector>
#include "ops.h"

Image::Image() {}
Image::Image(int w, int h, int c, std::vector<unsigned char> p) : width(w), height(h), pixels(p) {}
Image::~Image() {}

// void Image::convolution(std::vector<std::vector<float>> kernel) { ConvolutionOp::apply(*this, kernel); }
