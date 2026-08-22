#pragma once

#include <SFML/Graphics.hpp>
#include "image.h"
#include <string>

void display_image(const Image &img);
Image load_image(const std::string &filepath);
void save_image(const std::string &path, const Image &img);
