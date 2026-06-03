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
    src/window/window.cpp
    src/window/graphics/drawer.cpp
    src/window/graphics/helpers.cpp
    src/window/graphics/zindex.cpp
    src/window/controller/controller.cpp
    src/window/interface/interface.cpp
    src/window/interface/instruments.cpp
    src/window/interface/builder.cpp
    src/window/controller/callbackable.cpp
    src/context.cpp
)

target_include_directories(Geologos PRIVATE "external")
target_include_directories(Geologos PRIVATE src)

target_link_libraries(Geologos PRIVATE glfw OpenGL::GL glad)
