cmake_minimum_required(VERSION 3.2.0)
project(HighLoadCup)

SET(CMAKE_CXX_FLAGS "-O2  -Wall -Werror")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
SET(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS})

# removing throw an error in Clang Linux compilation


# Use libc++ and libc++abi instead of libstdc++ (only make sense on Linux with Clang)
# if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#    add_cxx_compiler_flag(-stdlib=libc++)
# endif()

set(LIB_SOURCE_FILES MainListener.cpp MainListener.hpp EntityRequestHandlerFactory.cpp EntityRequestHandlerFactory.hpp MessageGetter.cpp MessageGetter.hpp MessagesQueue.cpp MessagesQueue.hpp MessageAdder.cpp MessageAdder.hpp PingHandler.cpp PingHandler.hpp)

set(SOURCE_FILES main.cpp)


set (POCO_STATIC ON)
set (POCO_LIBRARY "../contrib/poco")
include_directories(../contrib/json)

include_directories(${POCO_LIBRARY}/Foundation/include)
include_directories(${POCO_LIBRARY}/Net/include)
include_directories(${POCO_LIBRARY}/Util/include)

add_library (Server ${LIB_SOURCE_FILES})
add_executable(HighLoadCup ${SOURCE_FILES})

find_package (Threads)

target_link_libraries(HighLoadCup Server PocoFoundation PocoUtil PocoNet ${CMAKE_THREAD_LIBS_INIT})
