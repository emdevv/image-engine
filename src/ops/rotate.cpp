#include "image.h"
#include "ops.h"
#include <stdexcept>
#include <vector>

void RotateOp::apply(Image &img, int angle) {
  // Normalize angle to handle negative numbers or numbers > 360 (e.g., -90
  // becomes 270)
  angle = ((angle % 360) + 360) % 360;

  if (angle != 90 && angle != 180 && angle != 270 && angle != 0) {
    throw std::invalid_argument(
        "Only 90, 180, and 270 degree rotations are supported right now!");
  }

  if (angle == 0)
    return; // Nothing to do

  const int channels = 4;
  int new_w = (angle == 180) ? img.width : img.height;
  int new_h = (angle == 180) ? img.height : img.width;

  std::vector<unsigned char> rotated_pixels(new_w * new_h * channels);

  for (int y = 0; y < img.height; ++y) {
    for (int x = 0; x < img.width; ++x) {
      int new_x = 0;
      int new_y = 0;

      // Coordinate mapping math based on angle
      if (angle == 90) {
        new_x = img.height - 1 - y;
        new_y = x;
      } else if (angle == 180) {
        new_x = img.width - 1 - x;
        new_y = img.height - 1 - y;
      } else if (angle == 270) {
        new_x = y;
        new_y = img.width - 1 - x;
      }

      // Map 2D coordinates back to 1D array positions
      size_t orig_idx = (y * img.width + x) * channels;
      size_t new_idx = (new_y * new_w + new_x) * channels;

      // Copy the RGBA channels
      for (int c = 0; c < channels; ++c) {
        rotated_pixels[new_idx + c] = img.pixels[orig_idx + c];
      }
    }
  }

  // Update the image object
  img.pixels = std::move(rotated_pixels);
  img.width = new_w;
  img.height = new_h;
}