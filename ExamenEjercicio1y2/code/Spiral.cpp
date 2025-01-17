///
/// Spiral.cpp
///

#include "Spiral.hpp"

Spiral::Spiral() {
    generateSpiral();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Spiral::~Spiral() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Spiral::generateSpiral() {
    const int totalVertices = 1000;
    for (int i = 0; i < totalVertices; ++i) {

        vertices.push_back(i);                 // Pasar los indices
    }
}

void Spiral::render() {
    glBindVertexArray(vao);
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size() / 3);
    glBindVertexArray(0);
}