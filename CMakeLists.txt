cmake_minimum_required(VERSION 3.27)
project(idwWithOpenCl)

set(CMAKE_CXX_STANDARD 17)

add_executable(idwWithOpenCl main.cpp
        cl.h
        IdwGpu.h
        io_utils.h)
