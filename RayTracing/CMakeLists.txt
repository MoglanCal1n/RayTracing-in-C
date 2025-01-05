cmake_minimum_required(VERSION 3.27)
project(RayTracing C)

set(CMAKE_C_STANDARD 23)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "C:/Users/Moglan Calin-Stefan/Downloads/SDL2-devel-2.30.11-mingw/SDL2-2.30.11/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

add_executable(RayTracing main.c)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
