#pragma once

#include <SFML/Graphics.hpp>
#include "image.h"
#include <string>

void display_image(const Image &img);
Image load_image(const std::string &filepath);
Image save_image(const std::string &path, const Image &img);
