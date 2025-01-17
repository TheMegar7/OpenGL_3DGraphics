///
/// Spiral.hpp
///

#ifndef SPIRAL_HPP
#define SPIRAL_HPP

#define _USE_MATH_DEFINES

#include <glad/glad.h>
#include <vector>
#include <cmath>

class Spiral {
private:
    GLuint vao;
    GLuint vbo;

    std::vector<GLfloat> vertices;

    void generateSpiral();

public:
    Spiral();
    ~Spiral();

    void render();
};

#endif