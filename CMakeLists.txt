cmake_minimum_required(VERSION 3.20)
project(image_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(IntelSYCL REQUIRED)

add_library(image_engine_core STATIC 
    src/ops/blur.cpp
    src/ops/convolution.cpp
    src/ops/crop.cpp
    src/ops/rotate.cpp
    src/executor.cpp
    src/image.cpp
    src/image_io.cpp
)

target_include_directories(image_engine_core PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/stb
    ${CMAKE_SOURCE_DIR}/external/CLI11
)

add_sycl_to_target(TARGET image_engine_core)

target_link_libraries(image_engine_core PRIVATE
    sfml-graphics sfml-window sfml-system
)

add_executable(image-engine
    src/main.cpp
    src/engine_logic.cpp
)

target_link_libraries(image-engine PRIVATE 
    image_engine_core 
)

add_subdirectory(tests)