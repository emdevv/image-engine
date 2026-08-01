#include "image.h"
#include "ops.h"
#include "sycl/queue.hpp"
#include <algorithm>
#include <vector>

void ConvolutionOp::apply_native(Image &img) {
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

void ConvolutionOp::apply_kernel(Image &img, sycl::queue &q) {
  const int channels = 4;
  const int img_w = img.width;
  const int img_h = img.height;

  std::vector<unsigned char> out(img.pixels.size());

  {
    sycl::buffer<unsigned char, 1> in_buf(img.pixels.data(), sycl::range<1>(img.pixels.size()));
    sycl::buffer<unsigned char, 1> out_buf(out.data(), sycl::range<1>(out.size()));

    sycl::buffer<float, 1> k_buf(flat_kernel.data(), sycl::range<1>(flat_kernel.size()));

    q.submit([&](sycl::handler &cgh) {
      auto in = in_buf.get_access<sycl::access::mode::read>(cgh);
      auto out = out_buf.get_access<sycl::access::mode::write>(cgh);
      auto k_acc = k_buf.get_access<sycl::access::mode::read>(cgh);

      int hH = halfH;
      int hW = halfW;
      int k_h = kh;
      int k_w = kw;

      cgh.parallel_for(sycl::range<2>(img_h, img_w), [=](sycl::id<2> idx) {
        int y = idx[0];
        int x = idx[1];
        float r = 0, g = 0, b = 0;

        for (int ky = -hH; ky <= hH; ky++) {
          for (int kx = -hW; kx <= hW; kx++) {

            int ny = sycl::clamp(y + ky, 0, img_h - 1);
            int nx = sycl::clamp(x + kx, 0, img_w - 1);

            float kval = k_acc[(ky + hH) * k_w + (kx + hW)];
            size_t src_idx = (ny * img_w + nx) * channels;

            r += in[src_idx + 0] * kval;
            g += in[src_idx + 1] * kval;
            b += in[src_idx + 2] * kval;
          }
        }

        size_t out_idx = (y * img_w + x) * channels;

        out[out_idx + 0] = static_cast<unsigned char>(sycl::clamp((int)r, 0, 255));
        out[out_idx + 1] = static_cast<unsigned char>(sycl::clamp((int)g, 0, 255));
        out[out_idx + 2] = static_cast<unsigned char>(sycl::clamp((int)b, 0, 255));

        out[out_idx + 3] = in[out_idx + 3];
      });
    });
  }

  img.pixels = std::move(out);
}
