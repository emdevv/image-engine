#include <gtest/gtest.h>
#include "executor.h"
#include "ops.h"
#include "image.h"

TEST(NativeCPU, RotateOp) {
  Image img;
  img.width = 2;
  img.height = 1;
  img.pixels = {1, 1, 1, 255, 2, 2, 2, 255};

  Executor exec(Device::NATIVE_CPU);
  RotateOp op(90);
  exec.execute(img, op);

  EXPECT_EQ(img.width, 1);
  EXPECT_EQ(img.height, 2);

  EXPECT_EQ(img.pixels[0], 1); // Pixel 0 R
  EXPECT_EQ(img.pixels[4], 2); // Pixel 1 R
}

TEST(SYCL_CPU, RotateOp) {
  Image img;
  img.width = 2;
  img.height = 1;
  img.pixels = {1, 1, 1, 255, 2, 2, 2, 255};

  Executor exec(Device::CPU);
  RotateOp op(90);
  exec.execute(img, op);

  EXPECT_EQ(img.width, 1);
  EXPECT_EQ(img.height, 2);

  EXPECT_EQ(img.pixels[0], 1); // Pixel 0 R
  EXPECT_EQ(img.pixels[4], 2); // Pixel 1 R
}

TEST(SYCL_GPU, RotateOp) {
  Image img;
  img.width = 2;
  img.height = 1;
  img.pixels = {1, 1, 1, 255, 2, 2, 2, 255};

  Executor exec(Device::GPU);
  RotateOp op(90);
  exec.execute(img, op);

  EXPECT_EQ(img.width, 1);
  EXPECT_EQ(img.height, 2);

  EXPECT_EQ(img.pixels[0], 1); // Pixel 0 R
  EXPECT_EQ(img.pixels[4], 2); // Pixel 1 R
}

TEST(NativeCPU, CropBottomRightPixel) {
  Image img;
  img.width = 2;
  img.height = 2;
  // 2x2 Image with RGBA (16 bytes total)
  // [0,0]: {1, 1, 1, 255} | [1,0]: {2, 2, 2, 255}
  // [0,1]: {3, 3, 3, 255} | [1,1]: {4, 4, 4, 255}
  img.pixels = {1, 1, 1, 255, 2, 2, 2, 255, 3, 3, 3, 255, 4, 4, 4, 255};

  Executor exec(Device::NATIVE_CPU);
  // Crop 1x1 at position x=1, y=1 (the pixel {4,4,4,255})
  CropOp op(1, 1, 1, 1);
  exec.execute(img, op);

  EXPECT_EQ(img.width, 1);
  EXPECT_EQ(img.height, 1);
  EXPECT_EQ(img.pixels[0], 4);
  EXPECT_EQ(img.pixels[1], 4);
  EXPECT_EQ(img.pixels[2], 4);
  EXPECT_EQ(img.pixels[3], 255);
}

TEST(SYCL_CPU, CropBottomRightPixel) {
  Image img;
  img.width = 2;
  img.height = 2;
  img.pixels = {1, 1, 1, 255, 2, 2, 2, 255, 3, 3, 3, 255, 4, 4, 4, 255};

  Executor exec(Device::CPU);
  CropOp op(1, 1, 1, 1);
  exec.execute(img, op);

  EXPECT_EQ(img.width, 1);
  EXPECT_EQ(img.height, 1);
  EXPECT_EQ(img.pixels[0], 4);
  EXPECT_EQ(img.pixels[1], 4);
  EXPECT_EQ(img.pixels[2], 4);
  EXPECT_EQ(img.pixels[3], 255);
}

TEST(SYCL_GPU, CropBottomRightPixel) {
  Image img;
  img.width = 2;
  img.height = 2;
  img.pixels = {1, 1, 1, 255, 2, 2, 2, 255, 3, 3, 3, 255, 4, 4, 4, 255};

  Executor exec(Device::GPU);
  CropOp op(1, 1, 1, 1);
  exec.execute(img, op);

  EXPECT_EQ(img.width, 1);
  EXPECT_EQ(img.height, 1);
  EXPECT_EQ(img.pixels[0], 4);
  EXPECT_EQ(img.pixels[1], 4);
  EXPECT_EQ(img.pixels[2], 4);
  EXPECT_EQ(img.pixels[3], 255);
}

TEST(NativeCPU, ConvolutionOp) {
  Image img;
  img.width = 3;
  img.height = 3;
  // Fill with 0s, set center pixel to {100, 100, 100, 255}
  img.pixels.resize(3 * 3 * 4, 0);
  img.pixels[4 * 4 + 0] = 100; // R
  img.pixels[4 * 4 + 1] = 100; // G
  img.pixels[4 * 4 + 2] = 100; // B
  img.pixels[4 * 4 + 3] = 255; // A

  Executor exec(Device::NATIVE_CPU);
  // Identity Kernel
  ConvolutionOp op({{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});
  exec.execute(img, op);

  // Check that the center pixel remains the same
  EXPECT_EQ(img.pixels[16], 100);
  EXPECT_EQ(img.pixels[17], 100);
  EXPECT_EQ(img.pixels[18], 100);
  EXPECT_EQ(img.pixels[19], 255);
}

TEST(SYCL_CPU, ConvolutionOp) {
  Image img;
  img.width = 3;
  img.height = 3;
  img.pixels.resize(3 * 3 * 4, 0);
  img.pixels[4 * 4 + 0] = 100;
  img.pixels[4 * 4 + 1] = 100;
  img.pixels[4 * 4 + 2] = 100;
  img.pixels[4 * 4 + 3] = 255;

  Executor exec(Device::CPU);
  ConvolutionOp op({{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});
  exec.execute(img, op);

  EXPECT_EQ(img.pixels[16], 100);
  EXPECT_EQ(img.pixels[17], 100);
  EXPECT_EQ(img.pixels[18], 100);
  EXPECT_EQ(img.pixels[19], 255);
}

TEST(SYCL_GPU, ConvolutionOp) {
  Image img;
  img.width = 3;
  img.height = 3;
  img.pixels.resize(3 * 3 * 4, 0);
  img.pixels[4 * 4 + 0] = 100;
  img.pixels[4 * 4 + 1] = 100;
  img.pixels[4 * 4 + 2] = 100;
  img.pixels[4 * 4 + 3] = 255;

  Executor exec(Device::GPU);
  ConvolutionOp op({{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});
  exec.execute(img, op);

  EXPECT_EQ(img.pixels[16], 100);
  EXPECT_EQ(img.pixels[17], 100);
  EXPECT_EQ(img.pixels[18], 100);
  EXPECT_EQ(img.pixels[19], 255);
}

TEST(NativeCPU, BlurOp) {
  Image img;
  img.width = 10;
  img.height = 10;
  // Initialize with 100 for ALL channels (R, G, B, A)
  img.pixels.assign(10 * 10 * 4, 100);

  Executor exec(Device::NATIVE_CPU);
  BlurOp op(50);
  exec.execute(img, op);

  // Expect all channels to remain 100 after the blur
  EXPECT_EQ(img.pixels[0], 100); // R
  EXPECT_EQ(img.pixels[3], 100); // A (Blurred)
}

TEST(SYCL_CPU, BlurOp) {
  Image img;
  img.width = 10;
  img.height = 10;
  img.pixels.assign(10 * 10 * 4, 100);

  Executor exec(Device::CPU);
  BlurOp op(50);
  exec.execute(img, op);

  EXPECT_EQ(img.pixels[0], 100);
  EXPECT_EQ(img.pixels[3], 100);
}

TEST(SYCL_GPU, BlurOp) {
  Image img;
  img.width = 10;
  img.height = 10;
  img.pixels.assign(10 * 10 * 4, 100);

  Executor exec(Device::GPU);
  BlurOp op(50);
  exec.execute(img, op);

  EXPECT_EQ(img.pixels[0], 100);
  EXPECT_EQ(img.pixels[3], 100);
}