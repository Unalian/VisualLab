cmake_minimum_required(VERSION 3.17)
project(visualLab3_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(./include)

add_executable(visualLab3_1 ColormapRenderer.cpp GlyphRenderer.cpp Grid.cpp sampleUniformGridGlyphs.cpp
        ScalarAttributes.cpp UniformGrid.cpp VectorAttributes.cpp)


if (APPLE)
    target_link_libraries(visualLab3_1 "-framework OpenGL")
    target_link_libraries(visualLab3_1 "-framework GLUT")
endif()

