#include "engine_config.h"
#include "image.h"
#include "load_image.h"
#include "display_image.h"
#include "ops.h"
#include "executor.h"
#include "types.h"
#include <print>

void setup_cli(CLI::App &app, AppContext &ctx) {
  app.require_subcommand(1);

  app.add_option("filepath", ctx.filepath, "Path to the image file")->required()->check(CLI::ExistingFile);

  ctx.crop_cmd = app.add_subcommand("crop", "Crop the image");
  ctx.crop_cmd->add_option("x", ctx.cx, "X")->required();
  ctx.crop_cmd->add_option("y", ctx.cy, "Y")->required();
  ctx.crop_cmd->add_option("width", ctx.cw, "Width")->required();
  ctx.crop_cmd->add_option("height", ctx.ch, "Height")->required();

  ctx.blur_cmd = app.add_subcommand("blur", "Blur the image");
  ctx.blur_cmd->add_option("percentage", ctx.blur_percentage, "Blur (0-100)")->required();

  ctx.rotate_cmd = app.add_subcommand("rotate", "Rotate the image");
  ctx.rotate_cmd->add_option("angle", ctx.rotate_angle, "Angle (90, 180, 270)")->required();

  auto device_group = app.add_option_group("Device", "Select compute device");
  device_group->add_flag_callback("--cpu", [&ctx]() { ctx.type = Device::CPU; }, "Use CPU");
  device_group->add_flag_callback("--gpu", [&ctx]() { ctx.type = Device::GPU; }, "Use GPU");
  device_group->add_flag_callback("--native-cpu", [&ctx]() { ctx.type = Device::NATIVE_CPU; }, "Use native CPU");
  device_group->add_flag_callback("--compare", [&ctx]() { ctx.type = Device::COMPARE; }, "Use all devices and create profiling");

  ctx.conv_cmd = app.add_subcommand("convolution", "Convolution to image");
}

void run_operations(const AppContext &ctx) {
  std::vector<std::vector<float>> kernel = {{-1.f, -1.f, -1.f}, {-1.f, 8.f, -1.f}, {-1.f, -1.f, -1.f}};

  Executor myExe(ctx.type);
  Image img = load_image(ctx.filepath);

  display_image(img);

  if (ctx.crop_cmd->parsed()) {
    myExe.execute(img, CropOp(ctx.cx, ctx.cy, ctx.cw, ctx.ch));
  } else if (ctx.blur_cmd->parsed()) {
    myExe.execute(img, BlurOp(ctx.blur_percentage));
  } else if (ctx.rotate_cmd->parsed()) {
    myExe.execute(img, RotateOp(ctx.rotate_angle));
  } else if (ctx.conv_cmd->parsed()) {
    myExe.execute(img, ConvolutionOp(kernel));
  }

  display_image(img);
}
