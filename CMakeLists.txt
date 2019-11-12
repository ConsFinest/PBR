cmake_minimum_required(VERSION 2.6)
project(PBR)

include_directories(src contrib)

add_library(PBRengine
	src/PBRengine/shaderProgram.cpp
	src/PBRengine/shaderProgram.h
)

target_link_libraries(PBRengine SDL2 opengl32 glew32)

add_executable(PBRdemo
	src/PBRdemo/demo.cpp
)

target_link_libraries(PBRdemo PBRengine)