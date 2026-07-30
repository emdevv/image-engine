#include "ops.h"
#include "image.h"
#include <stdexcept>

void RotateOp::apply_native(Image &img) {
  // Normalize angle to handle negative numbers or numbers > 360 (e.g., -90
  // becomes 270)
  angle = ((angle % 360) + 360) % 360;

  if (angle != 90 && angle != 180 && angle != 270 && angle != 0) {
    throw std::invalid_argument("Only 90, 180, and 270 degree rotations are supported right now!");
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

void RotateOp::apply_kernel(Image &img, sycl::queue &q) {

  // Normalize angle
  int local_angle = ((angle % 360) + 360) % 360;
  if (local_angle == 0)
    return;

  // Setup Dimensions
  const int channels = 4;
  const int img_w = img.width;
  const int img_h = img.height;
  int new_w = (local_angle == 180) ? img_w : img_h;
  int new_h = (local_angle == 180) ? img_h : img_w;

  // Prepare result buffer
  std::vector<unsigned char> rotated_pixels(new_w * new_h * channels);

  // Submit to SYCL queue
  {
    // Buffers manage the data transfer between host (CPU) and device (GPU/CPU)
    sycl::buffer<unsigned char, 1> input_buf(img.pixels.data(), sycl::range<1>(img.pixels.size()));
    sycl::buffer<unsigned char, 1> output_buf(rotated_pixels.data(), sycl::range<1>(rotated_pixels.size()));

    q.submit([&](sycl::handler &cgh) {
      // Get accessors to the buffer data
      sycl::accessor in(input_buf, cgh, sycl::read_only);
      sycl::accessor out(output_buf, cgh, sycl::write_only);

      // Execute kernel
      cgh.parallel_for(sycl::range<2>(new_h, new_w), [=](sycl::id<2> idx) {
        int out_y = idx[0];
        int out_x = idx[1];

        int in_x = 0;
        int in_y = 0;

        // Coordinate mapping math
        if (local_angle == 90) {
          in_x = out_y;
          in_y = img_w - 1 - out_x;
        } else if (local_angle == 180) {
          in_x = img_w - 1 - out_x;
          in_y = img_h - 1 - out_y;
        } else if (local_angle == 270) {
          in_x = img_h - 1 - out_y;
          in_y = out_x;
        }

        // Map 2D coordinates back to 1D array positions
        size_t orig_idx = (size_t)(in_y * img_w + in_x) * channels;
        size_t new_idx = (size_t)(out_y * new_w + out_x) * channels;

        // Copy the RGBA channels
        for (int c = 0; c < channels; ++c) {
          out[new_idx + c] = in[orig_idx + c];
        }
      });
    });
  }

  img.pixels = std::move(rotated_pixels);
  img.width = new_w;
  img.height = new_h;
}
