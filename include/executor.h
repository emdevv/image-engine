#pragma once
#include "image.h"
#include "types.h"
#include <sycl/sycl.hpp>

class Executor {
public:
  Executor(Device t);
  ~Executor();

  template <typename OpType>
  void execute(Image &img, OpType op) {
    if (type == Device::NATIVE_CPU) {
      op.apply_native(img);
    } else {
      op.apply_kernel(img, q);
    }
  }

private:
  Device type;
  sycl::queue q;
};
