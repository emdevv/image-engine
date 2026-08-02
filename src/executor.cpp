#include "sycl/info/info_desc.hpp"
#include "sycl/properties/queue_properties.hpp"

#include "executor.h"
#include "types.h"

#include <string>
#include <print>

std::string get_device_name_info(bool use_gpu) {
  try {
    sycl::device dev{use_gpu ? sycl::gpu_selector_v : sycl::cpu_selector_v};
    return dev.get_info<sycl::info::device::name>();
  } catch (sycl::exception const &e) {
    return "Unknown Device (No compatible device found)";
  }
}

Executor::Executor(Device t) : type(t) {
  if (type == Device::NATIVE_CPU) {
    std::print("[NATIVE Info] Using: {}\n", get_device_name_info(false));
    return;
  }

  auto prop = sycl::property::queue::enable_profiling();

  if (type == Device::COMPARE) {
    q = sycl::queue(sycl::cpu_selector_v, prop);
    q_s = sycl::queue(sycl::gpu_selector_v, prop);
    return;
  }

  try {
    if (type == Device::CPU) {
      q = sycl::queue(sycl::cpu_selector_v, prop);
    } else if (type == Device::GPU) {
      q = sycl::queue(sycl::gpu_selector_v, prop);
    }

    std::print("[SYCL Info] Using: {}\n", get_device_name_info(true));
  } catch (const sycl::exception &e) {
    std::cerr << "[SYCL Error] Requested device unavailable: " << e.what() << std::endl;
    // Fallback or exit depending on your requirements
    throw;
  }
}

Executor::~Executor() {}
