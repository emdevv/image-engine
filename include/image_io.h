#pragma once

#include <SFML/Graphics.hpp>
#include "image.h"
#include <string>

void display_image(const Image &img);
Image load_image(const std::string &filepath);
void save_image(const Image &img, const std::string &filename = "output.png");
