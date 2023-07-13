cmake_minimum_required(VERSION 3.22)
project(t44rt)

set(CMAKE_CXX_STANDARD 14)
set(OpenGlLinkers -lglut -lGLU -lGL)
add_executable(OpenGLLinuxTest1 t44rt.cpp frame.h ray.h rt.cpp rt.h sphere.h surface.h vec.h win.h) #common to all clion project

target_link_libraries(OpenGLLinuxTest1 ${OpenGlLinkers})

