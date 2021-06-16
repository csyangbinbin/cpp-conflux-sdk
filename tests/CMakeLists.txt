hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

add_executable(test_main test_main.cpp)
target_link_libraries(test_main cpp_conflux_sdk GTest::gtest_main)


