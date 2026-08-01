#include "image.h"
#include "ops.h"
#include "sycl/event.hpp"
#include "sycl/queue.hpp"
#include <algorithm>
#include <vector>

void BlurOp::apply_native(Image &img) {
  const int channels = 4;
  if (percentage == 0)
    return;

  if (percentage > 100)
    percentage = 100;

  // Cap the maximum radius to 10% of the smallest image dimension
  int max_radius = std::min(img.width, img.height) / 10;
  if (max_radius < 1)
    max_radius = 1;
  int radius = (percentage * max_radius) / 100;

  if (radius == 0)
    return;

  // Allocate intermediate and final buffers
  std::vector<unsigned char> temp_pixels(img.pixels.size());
  std::vector<unsigned char> blurred_pixels(img.pixels.size());

  auto start = std::chrono::high_resolution_clock::now();

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
      blurred_pixels[target_idx + 0] = static_cast<unsigned char>(sum_r / count);
      blurred_pixels[target_idx + 1] = static_cast<unsigned char>(sum_g / count);
      blurred_pixels[target_idx + 2] = static_cast<unsigned char>(sum_b / count);
      blurred_pixels[target_idx + 3] = static_cast<unsigned char>(sum_a / count);
    }
  }

  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> duration = end - start;

  std::cout << "[Profiling] BlurOp execution time: " << duration.count() << " ms" << std::endl;

  img.pixels = std::move(blurred_pixels);
}

void BlurOp::apply_kernel(Image &img, sycl::queue &q) {
  const int channels = 4;
  int blur_percentage = 50;

  int max_radius = std::min(img.width, img.height) / 10;
  if (max_radius < 1)
    max_radius = 1;
  int radius = (blur_percentage * max_radius) / 100;

  if (radius == 0)
    return;

  const int img_w = img.width;
  const int img_h = img.height;
  const int r = radius;

  std::vector<unsigned char> temp_pixels(img.pixels.size());

  {
    sycl::buffer<unsigned char, 1> in_buf(img.pixels.data(), sycl::range<1>(img.pixels.size()));
    sycl::buffer<unsigned char, 1> tmp_buf(temp_pixels.data(), sycl::range<1>(temp_pixels.size()));

    // --- PASS 1: Horizontal Blur ---
    sycl::event e1 = q.submit([&](sycl::handler &cgh) {
      auto in = in_buf.get_access<sycl::access::mode::read>(cgh);
      auto tmp = tmp_buf.get_access<sycl::access::mode::write>(cgh);

      cgh.parallel_for(sycl::range<2>(img_h, img_w), [=](sycl::id<2> idx) {
        int y = idx[0];
        int x = idx[1];
        int sum[4] = {0, 0, 0, 0};
        int count = 0;

        for (int kx = -r; kx <= r; ++kx) {
          int nx = x + kx;
          if (nx >= 0 && nx < img_w) {
            size_t src_idx = (y * img_w + nx) * channels;
            for (int c = 0; c < channels; ++c)
              sum[c] += in[src_idx + c];
            count++;
          }
        }

        size_t target_idx = (y * img_w + x) * channels;
        for (int c = 0; c < channels; ++c)
          tmp[target_idx + c] = static_cast<unsigned char>(sum[c] / count);
      });
    });

    // --- PASS 2: Vertical Blur ---
    sycl::event e2 = q.submit([&](sycl::handler &cgh) {
      auto tmp = tmp_buf.get_access<sycl::access::mode::read>(cgh);
      auto out = in_buf.get_access<sycl::access::mode::write>(cgh);

      cgh.parallel_for(sycl::range<2>(img_h, img_w), [=](sycl::id<2> idx) {
        int y = idx[0];
        int x = idx[1];
        int sum[4] = {0, 0, 0, 0};
        int count = 0;

        for (int ky = -r; ky <= r; ++ky) {
          int ny = y + ky;
          if (ny >= 0 && ny < img_h) {
            size_t src_idx = (ny * img_w + x) * channels;
            for (int c = 0; c < channels; ++c)
              sum[c] += tmp[src_idx + c];
            count++;
          }
        }

        size_t target_idx = (y * img_w + x) * channels;
        for (int c = 0; c < channels; ++c)
          out[target_idx + c] = static_cast<unsigned char>(sum[c] / count);
      });
    });

    // e2.wait() is enough
    e2.wait();

    auto start1 = e1.get_profiling_info<sycl::info::event_profiling::command_start>();
    auto end1 = e1.get_profiling_info<sycl::info::event_profiling::command_end>();

    auto start2 = e2.get_profiling_info<sycl::info::event_profiling::command_start>();
    auto end2 = e2.get_profiling_info<sycl::info::event_profiling::command_end>();

    double duration1_ms = (end1 - start1) / 1e6;
    double duration2_ms = (end2 - start2) / 1e6;
    double total_ms = duration1_ms + duration2_ms;

    std::cout << "[Profiling] BlurOp execution time: " << total_ms << " ms" << std::endl;
  }
}
