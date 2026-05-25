#include "image.h"
#include "ops.h"
#include <algorithm>
#include <stdexcept>
#include <vector>

void CropOp::apply(Image &img, int crop_x, int crop_y, int crop_w, int crop_h) {
  const int channels = 4;
  if (crop_x + crop_w > img.width || crop_y + crop_h > img.height ||
      crop_x < 0 || crop_y < 0) {
    throw std::out_of_range("Crop region goes out of the image boundaries!");
  }

  std::vector<unsigned char> crepped_pixels;
  crepped_pixels.resize(crop_w * crop_h * channels);

  for (int row = 0; row < crop_h; ++row) {
    // Find where this specific row starts in the ORIGINAL image
    int orig_y = crop_y + row;
    size_t orig_row_start_index = (orig_y * img.width + crop_x) * channels;

    // Find where this specific row starts in the NEW cropped image
    size_t crop_row_start_index = (row * crop_w) * channels;

    // How many bytes to copy for this row
    size_t bytes_to_copy = crop_w * channels;

    // Copy the contiguous block of pixels for this row
    std::copy(img.pixels.begin() + orig_row_start_index,
              img.pixels.begin() + orig_row_start_index + bytes_to_copy,
              crepped_pixels.begin() + crop_row_start_index);
  }

  img.pixels = std::move(crepped_pixels);
  img.width = crop_w;
  img.height = crop_h;
}