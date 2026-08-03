#include "image.h"
#include <vector>

Image::Image() {}
Image::Image(int w, int h, int c, std::vector<unsigned char> p) : width(w), height(h), pixels(p) {}
Image::~Image() {}
