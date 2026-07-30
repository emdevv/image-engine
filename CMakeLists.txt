cmake_minimum_required(VERSION 3.20)

project(image_engine LANGUAGES CXX)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(IntelSYCL REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
)

# Create executable
add_executable(image-engine ${SRC_FILES} ${IMGUI_SOURCES})

# 2. Add SYCL support
# This handles all necessary compile/link flags automatically
add_sycl_to_target(TARGET image-engine SOURCES ${SRC_FILES})

# 3. Include Directories
target_include_directories(image-engine PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/stb
    ${CMAKE_SOURCE_DIR}/external/CLI11
    ${IMGUI_DIR}
)

# 4. Link Libraries
target_link_libraries(image-engine PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)