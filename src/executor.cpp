#include "executor.h"
#include "types.h"

Executor::Executor(Device t) : type(t) {
  if (type == Device::NATIVE_CPU)
    return;

  try {
    if (type == Device::CPU) {
      q = sycl::queue(sycl::cpu_selector_v);
    } else if (type == Device::GPU) {
      q = sycl::queue(sycl::gpu_selector_v);
    }

    std::cout << "[SYCL Info] Using: " << q.get_device().get_info<sycl::info::device::name>() << std::endl;
  } catch (const sycl::exception &e) {
    std::cerr << "[SYCL Error] Requested device unavailable: " << e.what() << std::endl;
    // Fallback or exit depending on your requirements
    throw;
  }
}

Executor::~Executor() {}
