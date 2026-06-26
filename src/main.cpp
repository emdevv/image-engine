#include "display_image.h"
#include "load_image.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Usage ./image-engine <image_path>\n";
  }

  std::string filepath = argv[1];
  Image img = load_image(filepath);
  display_image(img);
  img.crop(30, 100, 310, 280);

  std::cout << "Image loaded: " << img.width << "x" << img.height << "\n";

  display_image(img);

  return 0;
}
