cmake_minimum_required(VERSION 3.10)

project(proxy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS$} -static")

set(SOURCES_FILES
    proxy.cpp
    main.cpp)
set(HEADER_FILES
    proxy.hpp)

add_executable(proxy ${SOURCES_FILES} ${HEADER_FILES})

set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

