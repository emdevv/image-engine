#include "display_image.h"
#include <SFML/Graphics.hpp>

void display_image(Image &img) {

  // Create SFML window
  sf::RenderWindow window(sf::VideoMode(800, 600), "Image Viewer");

  // Create SFML texture from your LoadedImage
  sf::Texture texture;
  texture.create(img.width, img.height);
  texture.update(img.pixels.data());

  sf::Sprite sprite(texture);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear(sf::Color::Black);
    // Center the sprite in the window
    sf::Vector2u winSize = window.getSize();
    sf::Vector2u imgSize = texture.getSize();

    sprite.setPosition((winSize.x - imgSize.x) * 0.5f,
                       (winSize.y - imgSize.y) * 0.5f);

    window.draw(sprite);
    window.display();
  }
}
