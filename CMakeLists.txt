cmake_minimum_required(VERSION 3.15)
project(Geologos)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

include_directories(src)

add_executable(Geologos
    src/main.cpp
    src/shapes/points.cpp
    src/shapes/builder.cpp
    src/window/graphics/drawer.cpp
    src/window/window.cpp
    src/window/controller/controller.cpp
    src/window/controller/controller.h)

target_include_directories(Geologos PRIVATE "external")
target_include_directories(Geologos PRIVATE src)

target_link_libraries(Geologos PRIVATE glfw OpenGL::GL glad)
