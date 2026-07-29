cmake_minimum_required(VERSION 3.20)

project(image_engine LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(IntelSYCL REQUIRED)

# Collect source files
file(GLOB APP_SOURCES "src/*.cpp" "src/ops/*.cpp")

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
)

add_executable(image-engine ${APP_SOURCES} ${IMGUI_SOURCES})

# Add SYCL support using Intel's modern CMake approach
add_sycl_to_target(TARGET image-engine SOURCES ${APP_SOURCES})

target_include_directories(image-engine PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/stb
    ${CMAKE_SOURCE_DIR}/external/CLI11
    ${IMGUI_DIR}
    ${IntelSYCL_INCLUDE_DIRS}
)

target_link_options(image-engine PRIVATE -fsycl)

target_link_libraries(image-engine PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)
