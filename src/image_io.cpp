#define STB_IMAGE_IMPLEMENTATION

#include "image.h"
#include "image_io.h"
#include <stb_image.h>
#include <print>
#include <stdexcept>
#include <string>
#include <filesystem>

void display_image(const Image &img) {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 0;

  // Create SFML window
  sf::RenderWindow window(sf::VideoMode(800, 600), "Image Viewer", sf::Style::Default, settings);

  window.setVerticalSyncEnabled(false);

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

    sprite.setPosition((winSize.x - imgSize.x) * 0.5f, (winSize.y - imgSize.y) * 0.5f);

    window.draw(sprite);
    window.display();
  }
}

Image load_image(const std::string &filepath) {
  Image img;

  // force RGBA
  unsigned char *data = stbi_load(filepath.c_str(), &img.width, &img.height, &img.channels, 4);

  // stbi function for throwing the full error
  if (!data) {
    std::print("Full error: {}\n", stbi_failure_reason());
    throw std::runtime_error("Failed to load image: " + filepath);
  }

  img.channels = 4;
  img.pixels.assign(data, data + img.width * img.height * img.channels);

  stbi_image_free(data);
  return img;
}

void save_image(const Image &img, const std::string &filename) {
  if (img.pixels.empty() || img.width <= 0 || img.height <= 0) {
    throw std::runtime_error("Empty image, abort!");
  }

  std::filesystem::path output_dir = "output";
  std::filesystem::path full_path = output_dir / filename;

  std::filesystem::create_directories(output_dir);

  if (std::filesystem::exists(full_path)) {
    std::filesystem::remove(full_path);
  }

  sf::Image sfml_img;
  sfml_img.create(img.width, img.height, img.pixels.data());

  if (!sfml_img.saveToFile(full_path.string())) {
    throw std::runtime_error("Saving error in: " + full_path.string());
  }
}
