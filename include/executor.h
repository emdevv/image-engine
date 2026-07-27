#pragma once
#include "image.h"
#include "types.h"

class Executor {
public:
  Executor(Device t);
  ~Executor();

  template <typename OpType>
  void execute(Image &img, OpType op) {
    if (type == Device::NATIVE_CPU) {
      op.apply_native(img);
    } else {
      op.apply_kernel(img);
    }
  }

private:
  Device type;
};