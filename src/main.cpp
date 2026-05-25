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
  img.crop(2, 3, 4, 5);

  std::cout << "Image loaded: " << img.width << "x" << img.height << "\n";

  // Create SFML window
  sf::RenderWindow window(sf::VideoMode(800, 600), "Image Viewer");

  // Create SFML texture from your LoadedImage
  sf::Texture texture;
  texture.create(img.width, img.height);
  texture.update(img.pixels.data());

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear(sf::Color::Black);
    display_image(window, texture);
    window.display();
  }

  return 0;
}
