#include "image.h"
#include "ops.h"
#include <algorithm>
#include <vector>

void ConvolutionOp::apply(Image &img, std::vector<std::vector<float>> kernel) {
  const int channels = 4;

  int kh = kernel.size();
  int kw = kernel[0].size();

  int halfH = kh / 2;
  int halfW = kw / 2;

  std::vector<unsigned char> out(img.pixels.size());

  for (int y = 0; y < img.height; y++) {
    for (int x = 0; x < img.width; x++) {

      // 1. Separate the channels mathematically
      float r = 0, g = 0, b = 0;

      for (int ky = -halfH; ky <= halfH; ky++) {
        for (int kx = -halfW; kx <= halfW; kx++) {

          int ny = std::clamp(y + ky, 0, img.height - 1);
          int nx = std::clamp(x + kx, 0, img.width - 1);

          float kval = kernel[ky + halfH][kx + halfW];
          size_t idx = (ny * img.width + nx) * channels;

          // Convolve ONLY color channels
          r += img.pixels[idx + 0] * kval;
          g += img.pixels[idx + 1] * kval;
          b += img.pixels[idx + 2] * kval;
        }
      }

      size_t out_idx = (y * img.width + x) * channels;

      // 2. Add an optional brightness bias to prevent negative edges from
      // clipping to 0 Set to 128 if you want a true mid-gray edge map, or 0 if
      // you are using a sharpening kernel
      float bias = 0.0f;

      out[out_idx + 0] = std::clamp((int)(r + bias), 0, 255);
      out[out_idx + 1] = std::clamp((int)(g + bias), 0, 255);
      out[out_idx + 2] = std::clamp((int)(b + bias), 0, 255);

      // 3. DO NOT CONVOLVE ALPHA. Explicitly pass through the original opacity!
      out[out_idx + 3] = img.pixels[out_idx + 3];
    }
  }
  img.pixels = std::move(out);
}