#include "image.h"

class CropOp {
public:
  static void apply(Image &img, int crop_x, int crop_y, int crop_w, int crop_h);
};

class BlurOp {
public:
  static void apply(Image &img, int blur);
};