cmake_minimum_required(VERSION 3.17)
project(visualLab2_1)

set(CMAKE_CXX_STANDARD 14)


add_executable(visualLab2_1 Grid.cpp
        ColormapRenderer.cpp
        sampleUniformGridScalars.cpp
        ScalarAttributes.cpp
        UniformGrid.cpp
        Renderer.h)


if (APPLE)
    target_link_libraries(visualLab2_1 "-framework OpenGL")
    target_link_libraries(visualLab2_1 "-framework GLUT")
endif()


