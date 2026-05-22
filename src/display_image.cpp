#include "display_image.h"
#include <SFML/Graphics.hpp>

void display_image(sf::RenderWindow &window, const sf::Texture &texture) {
  sf::Sprite sprite(texture);

  // Center the sprite in the window
  sf::Vector2u winSize = window.getSize();
  sf::Vector2u imgSize = texture.getSize();

  sprite.setPosition((winSize.x - imgSize.x) * 0.5f,
                     (winSize.y - imgSize.y) * 0.5f);

  window.draw(sprite);
}
