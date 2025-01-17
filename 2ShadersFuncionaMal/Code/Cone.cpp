///
/// Cone.cpp
///

#include "Cone.hpp"

Cone::Cone(float radius, float height, int segments) : radius(radius), height(height), segments(segments) {
    generateGeometry();

    glGenVertexArrays(1, &vao);
    glGenBuffers(VBO_COUNT, vbo_ids);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
    glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(GLfloat), coordinates.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

Cone::~Cone() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(VBO_COUNT, vbo_ids);
}

void Cone::generateGeometry() {
    float angleStep = 2.0f * M_PI / segments;

    for (int i = 0; i < segments; ++i) {
        float theta = i * angleStep;
        float nextTheta = (i + 1) * angleStep;

        coordinates.push_back(0.0f);
        coordinates.push_back(height);
        coordinates.push_back(0.0f);

        coordinates.push_back(radius * cos(theta));
        coordinates.push_back(0.0f);
        coordinates.push_back(radius * sin(theta));

        coordinates.push_back(radius * cos(nextTheta));
        coordinates.push_back(0.0f);
        coordinates.push_back(radius * sin(nextTheta));


        for (int j = 0; j < 3; ++j) {
            if (j != 0) colors.push_back(2.0f);
            else colors.push_back(0.0f);
            colors.push_back(0.5f);
            colors.push_back(0.0f);
        }

        indices.push_back(i * 3);
        indices.push_back(i * 3 + 1);
        indices.push_back(i * 3 + 2);
    }

    coordinates.push_back(0.0f);
    coordinates.push_back(0.0f);
    coordinates.push_back(0.0f);

    for (int i = 0; i < segments; ++i) {

        indices.push_back(coordinates.size() / 3);
        indices.push_back(i * 3 + 2);
        indices.push_back(i * 3 + 1);
    }


}

void Cone::render() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
