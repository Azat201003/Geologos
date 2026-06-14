cmake_minimum_required(VERSION 3.15)
project(Geologos)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

include_directories(source)

add_executable(Geologos
	source/main.cpp
	source/context.cpp
	source/shapes/points.cpp
	source/shapes/builder.cpp
	source/window/window.cpp
	source/window/graphics/text.cpp
	source/window/graphics/drawer.cpp
	source/window/graphics/zindex.cpp
	source/window/graphics/matrix.cpp
	source/window/graphics/helpers.cpp
	source/window/graphics/shaders.cpp
	source/window/interface/builder.cpp
	source/window/interface/interface.cpp
	source/window/interface/instruments.cpp
	source/window/controller/controller.cpp
	source/window/controller/callbackable.cpp
)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_compile_options(Geologos PRIVATE 
		-Wall 
		-Wextra 
		-Wdocumentation # Clang-specific warning
		-fcolor-diagnostics
		-Weverything
	)
endif()

target_include_directories(Geologos PRIVATE "external")
target_include_directories(Geologos PRIVATE source)

target_link_libraries(Geologos PRIVATE glfw OpenGL::GL glad Freetype::Freetype spdlog fmt)
