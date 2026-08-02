#include "image.h"
#include "ops.h"
#include "sycl/queue.hpp"

#include <algorithm>
#include <stdexcept>
#include <vector>
#include <print>

void CropOp::apply_native(Image &img) {
  const int channels = 4;
  if (crop_x + crop_w > img.width || crop_y + crop_h > img.height || crop_x < 0 || crop_y < 0) {
    throw std::out_of_range("Crop region goes out of the image boundaries!");
  }

  std::vector<unsigned char> crepped_pixels;
  crepped_pixels.resize(crop_w * crop_h * channels);

  auto start = std::chrono::high_resolution_clock::now();

  for (int row = 0; row < crop_h; ++row) {
    // Find where this specific row starts in the ORIGINAL image
    int orig_y = crop_y + row;
    size_t orig_row_start_index = (orig_y * img.width + crop_x) * channels;

    // Find where this specific row starts in the NEW cropped image
    size_t crop_row_start_index = (row * crop_w) * channels;

    // How many bytes to copy for this row
    size_t bytes_to_copy = crop_w * channels;

    // Copy the contiguous block of pixels for this row
    std::copy(img.pixels.begin() + orig_row_start_index, img.pixels.begin() + orig_row_start_index + bytes_to_copy,
              crepped_pixels.begin() + crop_row_start_index);
  }

  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> duration = end - start;

  std::cout << "[Profiling][Native CPU] CropOp execution time: " << duration.count() << " ms" << std::endl;

  img.pixels = std::move(crepped_pixels);
  img.width = crop_w;
  img.height = crop_h;
}

void CropOp::apply_kernel(Image &img, sycl::queue &q) {
  const int channels = 4;

  if (crop_x + crop_w > img.width || crop_y + crop_h > img.height || crop_x < 0 || crop_y < 0) {
    throw std::out_of_range("Crop region goes out of the image boundaries!");
  }

  std::vector<unsigned char> cropped_pixels(crop_w * crop_h * channels);

  const int c_x = crop_x;
  const int c_y = crop_y;
  const int c_w = crop_w;
  const int c_h = crop_h;
  const int old_w = img.width;

  {
    sycl::buffer<unsigned char, 1> in_buf(img.pixels.data(), sycl::range<1>(img.pixels.size()));
    sycl::buffer<unsigned char, 1> out_buf(cropped_pixels.data(), sycl::range<1>(cropped_pixels.size()));

    sycl::event e = q.submit([&](sycl::handler &cgh) {
      auto in = in_buf.get_access<sycl::access::mode::read>(cgh);
      auto out = out_buf.get_access<sycl::access::mode::write>(cgh);

      cgh.parallel_for(sycl::range<2>(c_h, c_w), [=](sycl::id<2> idx) {
        int out_y = idx[0];
        int out_x = idx[1];

        int in_x = c_x + out_x;
        int in_y = c_y + out_y;

        size_t in_idx = (in_y * old_w + in_x) * channels;
        size_t out_idx = (out_y * c_w + out_x) * channels;

        for (int c = 0; c < channels; ++c) {
          out[out_idx + c] = in[in_idx + c];
        }
      });
    });
    e.wait();

    auto start = e.get_profiling_info<sycl::info::event_profiling::command_start>();
    auto end = e.get_profiling_info<sycl::info::event_profiling::command_end>();

    double duration_ms = (end - start) / 1e6;

    std::print("[Profiling][{}] CropOp execution time: {} ms \n", q.get_device().get_info<sycl::info::device::name>(), duration_ms);
  }

  img.pixels = std::move(cropped_pixels);
  img.width = c_w;
  img.height = c_h;
}
