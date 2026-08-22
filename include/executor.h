#pragma once
#include "image.h"
#include "types.h"
#include <sycl/sycl.hpp>

class Executor {
public:
  Executor(Device t);
  ~Executor();

  template <typename OpType>
  Image execute(Image &img, OpType op) {
    Image out;

    if (type == Device::NATIVE_CPU) {
      op.apply_native(img, out);

    } else if (type == Device::COMPARE) {
      op.apply_native(img, out);
      op.apply_kernel(img, q, out);
      op.apply_kernel(img, q_s, out);

    } else {
      op.apply_kernel(img, q, out);
    }

    return out;
  }

private:
  Device type;
  sycl::queue q;
  sycl::queue q_s;
};
