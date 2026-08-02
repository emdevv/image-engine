#include <gtest/gtest.h>
#include "executor.h"
#include "ops.h"
#include "image.h"

TEST(RotateTest, Rotate90Degrees) {
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