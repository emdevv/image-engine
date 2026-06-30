#include "image.h"
#include "ops.h"
#include <algorithm>
#include <vector>

void BlurOp::apply(Image &img, int blur_percentage) {
  const int channels = 4;
  if (blur_percentage == 0)
    return;

  if (blur_percentage > 100)
    blur_percentage = 100;

  // Cap the maximum radius to 10% of the smallest image dimension
  int max_radius = std::min(img.width, img.height) / 10;
  if (max_radius < 1)
    max_radius = 1;
  int radius = (blur_percentage * max_radius) / 100;

  if (radius == 0)
    return;

  // Allocate intermediate and final buffers
  std::vector<unsigned char> temp_pixels(img.pixels.size());
  std::vector<unsigned char> blurred_pixels(img.pixels.size());

  // PASS 1: Horizontal Blur (img.pixels -> temp_pixels)
  for (int y = 0; y < img.height; ++y) {
    for (int x = 0; x < img.width; ++x) {
      int sum_r = 0, sum_g = 0, sum_b = 0, sum_a = 0;
      int count = 0;

      for (int kx = -radius; kx <= radius; ++kx) {
        int nx = x + kx;
        // Clamp horizontal edges safely
        if (nx >= 0 && nx < img.width) {
          size_t idx = (y * img.width + nx) * channels;
          sum_r += img.pixels[idx + 0];
          sum_g += img.pixels[idx + 1];
          sum_b += img.pixels[idx + 2];
          sum_a += img.pixels[idx + 3];
          count++;
        }
      }

      size_t target_idx = (y * img.width + x) * channels;
      temp_pixels[target_idx + 0] = static_cast<unsigned char>(sum_r / count);
      temp_pixels[target_idx + 1] = static_cast<unsigned char>(sum_g / count);
      temp_pixels[target_idx + 2] = static_cast<unsigned char>(sum_b / count);
      temp_pixels[target_idx + 3] = static_cast<unsigned char>(sum_a / count);
    }
  }

  // PASS 2: Vertical Blur (temp_pixels -> blurred_pixels)
  for (int y = 0; y < img.height; ++y) {
    for (int x = 0; x < img.width; ++x) {
      int sum_r = 0, sum_g = 0, sum_b = 0, sum_a = 0;
      int count = 0;

      for (int ky = -radius; ky <= radius; ++ky) {
        int ny = y + ky;
        // Clamp vertical edges safely
        if (ny >= 0 && ny < img.height) {
          size_t idx = (ny * img.width + x) * channels;
          sum_r += temp_pixels[idx + 0];
          sum_g += temp_pixels[idx + 1];
          sum_b += temp_pixels[idx + 2];
          sum_a += temp_pixels[idx + 3];
          count++;
        }
      }

      size_t target_idx = (y * img.width + x) * channels;
      blurred_pixels[target_idx + 0] =
          static_cast<unsigned char>(sum_r / count);
      blurred_pixels[target_idx + 1] =
          static_cast<unsigned char>(sum_g / count);
      blurred_pixels[target_idx + 2] =
          static_cast<unsigned char>(sum_b / count);
      blurred_pixels[target_idx + 3] =
          static_cast<unsigned char>(sum_a / count);
    }
  }

  img.pixels = std::move(blurred_pixels);
}