find_package(GTest REQUIRED)

add_executable(unittests unit_tests.cpp)

set_target_properties(unittests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

target_link_libraries(unittests PRIVATE 
    image_engine_core 
    GTest::gtest_main
)

add_sycl_to_target(TARGET unittests SOURCES unit_tests.cpp)
