cmake_minimum_required(VERSION 3.20)
project(image_engine LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
file(GLOB APP_SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/ops/*.cpp"
)

# ImGui (core only for now – no GLFW/OpenGL backends)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
)

add_executable(image-engine
    ${APP_SOURCES}
    ${IMGUI_SOURCES}
)

target_include_directories(image-engine PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/stb
    ${CMAKE_SOURCE_DIR}/external/CLI11
    ${IMGUI_DIR}
)

# SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

target_link_libraries(image-engine PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)
