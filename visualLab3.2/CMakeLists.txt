cmake_minimum_required(VERSION 3.17)
project(visualLab3_2)

set(CMAKE_CXX_STANDARD 14)

include_directories(./include)

add_executable(visualLab3_2 ColormapRenderer.cpp DirCodingRenderer.cpp GlyphRenderer.cpp Grid.cpp
        sampleDirColorCoding.cpp ScalarAttributes.cpp UniformGrid.cpp VectorAttributes.cpp)


if (APPLE)
    target_link_libraries(visualLab3_2 "-framework OpenGL")
    target_link_libraries(visualLab3_2 "-framework GLUT")
endif()

