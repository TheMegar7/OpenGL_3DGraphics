#include "Plane.hpp"

const GLfloat Plane::coordinates[] = {
    -1.0f, 0.0f, 1.0f,
     1.0f, 0.0f, 1.0f,
     1.0f, 0.0f,  -1.0f,
    -1.0f, 0.0f,  -1.0f
};

const GLfloat Plane::colors[] = {
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f
};

const GLuint Plane::indices[] = {
    0, 3, 1,
    1, 3, 2,
    0, 1, 3,
    1, 2, 3
};

Plane::Plane() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(VBO_COUNT, vbo_ids);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

Plane::~Plane() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(VBO_COUNT, vbo_ids);
}


void Plane::render() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
