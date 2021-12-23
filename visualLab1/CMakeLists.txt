cmake_minimum_required(VERSION 3.17)
project(visualLab1)

set(CMAKE_CXX_STANDARD 14)

include_directories(./include)

add_executable(visualLab1 main.cpp)

if (APPLE)
    target_link_libraries(visualLab1 "-framework OpenGL")
    target_link_libraries(visualLab1 "-framework GLUT")
endif()


