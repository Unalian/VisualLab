cmake_minimum_required(VERSION 3.17)
project(visualLab2)



set(CMAKE_CXX_STANDARD 14)

include_directories(./include)

set(CMAKE_CXX_STANDARD 98)

add_executable(visualLab2 sampleIsosurface.cpp Grid.cpp Image.cpp Isosurface.cpp MeshRenderer.cpp ScalarAttributes.cpp
        UnstructuredGrid.cpp UnstructuredGrid3D.cpp Volume.cpp zpr.cpp)


if (APPLE)
    target_link_libraries(visualLab2 "-framework OpenGL")
    target_link_libraries(visualLab2 "-framework GLUT")
endif()