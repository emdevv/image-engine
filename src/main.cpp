#include "display_image.h"
#include "load_image.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cout << "Usage ./image-engine <image_path> <crop>\n";
  }

  std::string filepath = argv[1];
  std::string operation = argv[2];

  Image img = load_image(filepath);

  if (operation == "crop") {
    if (argc < 7) {
      std::cout << "Crop op should have 4 inputs x,y,w,h \n";
      return 0;
    } else {
      display_image(img);
      img.crop(std::stoi(argv[3]), std::stoi(argv[4]), std::stoi(argv[5]),
               std::stoi(argv[6]));
      // img.crop(30, 100, 310, 280);
    }
  }

  std::cout << "Image loaded: " << img.width << "x" << img.height << "\n";
  display_image(img);

  return 0;
}
