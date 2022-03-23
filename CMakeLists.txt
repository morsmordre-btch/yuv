cmake_minimum_required(VERSION 3.5)

project(yuv LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(yuv main.cpp bitmapfile.h bitmapfile.cpp yuvfile.h yuvfile.cpp)
