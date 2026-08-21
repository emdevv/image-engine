#pragma once
#include "CLI11.hpp"
#include "types.h"
#include <string>

struct AppContext {
  std::string filepath;
  Device type = Device::NATIVE_CPU;

  int cx = 0, cy = 0, cw = 0, ch = 0;
  int blur_percentage = 0;
  int rotate_angle = 0;

  CLI::App *crop_cmd = nullptr;
  CLI::App *blur_cmd = nullptr;
  CLI::App *rotate_cmd = nullptr;
  CLI::App *conv_cmd = nullptr;
};

void setup_cli(CLI::App &app, AppContext &ctx);
void run_operations(const AppContext &ctx);