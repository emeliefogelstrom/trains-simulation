cmake_minimum_required(VERSION 3.20)
project(Trains VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(trains src/main.cpp src/Station.cpp)

# --- Tests ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(trains_tests
  tests/track_test.cpp
  tests/vehicleEscrow_test.cpp
)
target_include_directories(trains_tests PRIVATE include)
target_link_libraries(trains_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(trains_tests)