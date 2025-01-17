
#ifndef PLANE_HPP
#define PLANE_HPP

#include <glad/glad.h>

class Plane {
private:
    enum {
        COORDINATES_VBO,
        COLORS_VBO,
        INDICES_EBO,
        VBO_COUNT
    };

    static const GLfloat coordinates[];
    static const GLfloat colors[];
    static const GLuint indices[];

    GLuint vao;
    GLuint vbo_ids[VBO_COUNT];

public:
    Plane();
    ~Plane();

    void render();
};

#endif