///
/// Cylinder.hpp
///

#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#define _USE_MATH_DEFINES

#include <glad/glad.h>
#include <vector>
#include <cmath>

class Cylinder {
private:
    enum {
        COORDINATES_VBO,
        COLORS_VBO,
        INDICES_EBO,
        VBO_COUNT
    };

    float radius;
    float height;
    int segments;

    GLuint vao;
    GLuint vbo_ids[VBO_COUNT];

    std::vector<GLfloat> coordinates;
    std::vector<GLfloat> colors;
    std::vector<GLuint> indices;

    void generateGeometry();

public:
    Cylinder(float radius = 0.5f, float height = 1.0f, int segments = 36);
    ~Cylinder();

    void render();
};

#endif