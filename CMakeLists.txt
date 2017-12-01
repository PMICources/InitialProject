cmake_minimum_required(VERSION 3.2.0)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")


add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(contrib/gtest)
add_subdirectory(contrib/poco)

# add_test (UnitTests runTests)
