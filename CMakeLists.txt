cmake_minimum_required(VERSION 2.6)
project(PBRdemo)

include_directories(src contrib)

add_library(PBR
	src/PBR/core.cpp
	src/PBR/core.h
	src/PBR/resources.cpp
	src/PBR/resources.h
	src/PBR/shader.cpp
	src/PBR/shader.h
	src/PBR/exception.cpp
	src/PBR/exception.h
	src/PBR/texture.cpp
	src/PBR/texture.h
	src/PBR/camera.cpp
	src/PBR/camera.h
	src/PBR/renderTexture.cpp
	src/PBR/renderTexture.h
	src/PBR/3Dshapes.cpp
	src/PBR/3Dshapes.h
)

target_link_libraries(PBR glew32 opengl32 SDL2)


add_executable(Demo
	src/Demo/demo.cpp
)

target_link_libraries(Demo PBR)