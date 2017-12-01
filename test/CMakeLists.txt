cmake_minimum_required(VERSION 3.2.0)


set(CMAKE_CXX_FLAGS "-O2 -Wall -Werror ")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Locate GTest
set (GMOCK_DIR "../contrib/gtest/googlemock")
set (GTEST_DIR "../contrib/gtest/googletest")
include_directories(../contrib/json)
include_directories(../)
include_directories(${GMOCK_DIR}/include ${GTEST_DIR}/include)

set (POCO_STATIC ON)

include_directories(${POCO_LIBRARY}/Foundation/include)
include_directories(${POCO_LIBRARY}/Net/include)
include_directories(${POCO_LIBRARY}/Util/include)

find_package (Threads)

include_directories(../src)

# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests src/RunAllTests.cpp src/TestRequestFactory.cpp src/Mocks.hpp src/TestHandlers.cpp src/Mocks.cpp src/MultithreadTests.cpp)

target_link_libraries(runTests Server gtest gmock PocoFoundation PocoUtil PocoNet ${CMAKE_THREAD_LIBS_INIT})
