#include "CLI11.hpp"
#include "display_image.h"
#include "load_image.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main(int argc, char **argv) {
  CLI::App app{"SFML Image Engine"};
  app.require_subcommand(1); // Forces user to pick crop, blur, or rotate

  // 1. Global Option (Every command needs an image path)
  std::string filepath;
  app.add_option("filepath", filepath, "Path to the image file")
      ->required()
      ->check(CLI::ExistingFile); // Automatically checks if file exists!

  // 2. Crop Subcommand
  auto *crop_cmd = app.add_subcommand("crop", "Crop the image");
  int cx = 0, cy = 0, cw = 0, ch = 0;
  crop_cmd->add_option("x", cx, "X coordinate")->required();
  crop_cmd->add_option("y", cy, "Y coordinate")->required();
  crop_cmd->add_option("width", cw, "Width")->required();
  crop_cmd->add_option("height", ch, "Height")->required();

  // 3. Blur Subcommand
  auto *blur_cmd = app.add_subcommand("blur", "Blur the image");
  int blur_percentage = 0;
  blur_cmd->add_option("percentage", blur_percentage, "Blur percentage (0-100)")
      ->required();

  // 4. Rotate Subcommand
  auto *rotate_cmd = app.add_subcommand("rotate", "Rotate the image");
  int angle = 0;
  rotate_cmd->add_option("angle", angle, "Rotation angle (90, 180, 270)")
      ->required();

  // Parse the CLI arguments. If the user passes wrong inputs,
  // CLI11 prints an error and safely exits here.
  CLI11_PARSE(app, argc, argv);

  // --- Core Execution Logic ---
  Image img = load_image(filepath);
  std::cout << "Original Image loaded: " << img.width << "x" << img.height
            << "\n";

  // Show initial image
  display_image(img);

  if (crop_cmd->parsed()) {
    std::cout << "Applying crop...\n";
    img.crop(cx, cy, cw,
             ch); // No std::stoi needed! CLI11 already converted it to ints.
  } else if (blur_cmd->parsed()) {
    std::cout << "Applying blur...\n";
    img.blur(blur_percentage);
  } else if (rotate_cmd->parsed()) {
    std::cout << "Applying rotate...\n";
    img.rotate(angle);
  }

  std::cout << "Processed Image size: " << img.width << "x" << img.height
            << "\n";
  display_image(img);

  return 0;
}