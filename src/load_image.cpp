#define STB_IMAGE_IMPLEMENTATION
#include "load_image.h"
#include "image.h"
#include <iostream>
#include <stb_image.h>
#include <stdexcept>

Image load_image(const std::string &filepath) {
  Image img;

  // force RGBA
  unsigned char *data =
      stbi_load(filepath.c_str(), &img.width, &img.height, &img.channels, 4);

  // stbi function for throwing the full error
  if (!data) {
    std::cout << "Full error: " << stbi_failure_reason();
    throw std::runtime_error("Failed to load image: " + filepath);
  }

  img.channels = 4;
  img.pixels.assign(data, data + img.width * img.height * img.channels);

  stbi_image_free(data);
  return img;
}
