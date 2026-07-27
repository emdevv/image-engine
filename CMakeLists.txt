cmake_minimum_required(VERSION 3.20)

project(image_engine LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Collect source files
file(GLOB APP_SOURCES "src/*.cpp" "src/ops/*.cpp")

# 3. ImGui (doar fișierele .cpp)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
)

# 4. Definim executabilul
add_executable(image-engine ${APP_SOURCES} ${IMGUI_SOURCES})

# 5. Configurare include-uri
target_include_directories(image-engine PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/external/stb
    ${CMAKE_SOURCE_DIR}/external/CLI11
    ${IMGUI_DIR}
)

# --- AICI ESTE MAGIA (Surgicală) ---

# NU mai folosim target_compile_options global! 
# Aplicăm SYCL doar pe fișierul care chiar are cod GPU
set_source_files_properties(src/ops/rotate.cpp PROPERTIES COMPILE_FLAGS "-fsycl")

# Dar menținem link-ul de SYCL pentru executabil (ca să găsească bibliotecile)
target_link_options(image-engine PRIVATE -fsycl)

# 6. Linkare biblioteci
target_link_libraries(image-engine PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)