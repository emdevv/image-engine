#include "executor.h"
#include "sycl/info/info_desc.hpp"
#include "sycl/properties/queue_properties.hpp"
#include "types.h"
#include <print>

Executor::Executor(Device t) : type(t) {
  if (type == Device::NATIVE_CPU) {
    std::print("[NATIVE Info] Using: CPU \n");
    return;
  }

  auto prop = sycl::property::queue::enable_profiling();

  try {
    if (type == Device::CPU) {
      q = sycl::queue(sycl::cpu_selector_v, prop);
    } else if (type == Device::GPU) {
      q = sycl::queue(sycl::gpu_selector_v, prop);
    }

    std::print("[SYCL Info] Using: {} \n", q.get_device().get_info<sycl::info::device::name>());
  } catch (const sycl::exception &e) {
    std::cerr << "[SYCL Error] Requested device unavailable: " << e.what() << std::endl;
    // Fallback or exit depending on your requirements
    throw;
  }
}

Executor::~Executor() {}
