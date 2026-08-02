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

    } else if (type == Device::COMPARE) {
      op.apply_native(img);
      op.apply_kernel(img, q);
      op.apply_kernel(img, q_s);

    } else {
      op.apply_kernel(img, q);
    }
  }

private:
  Device type;
  sycl::queue q;
  sycl::queue q_s;
};
