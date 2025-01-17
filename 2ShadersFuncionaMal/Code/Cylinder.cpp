///
/// Cylinder.cpp
///

#include "Cylinder.hpp"

Cylinder::Cylinder(float radius, float height, int segments) : radius(radius), height(height), segments(segments) {
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

Cylinder::~Cylinder() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(VBO_COUNT, vbo_ids);
}

void Cylinder::generateGeometry() {
    float angleStep = 2.0f * M_PI / segments;

    // Colores del arcoíris para 36 segmentos
    std::vector<std::vector<GLfloat>> rainbow_colors = {
        {1.0f, 0.0f, 0.0f}, // Rojo
        {1.0f, 0.5f, 0.0f}, // Naranja
        {1.0f, 1.0f, 0.0f}, // Amarillo
        {0.0f, 1.0f, 0.0f}, // Verde
        {0.0f, 0.0f, 1.0f}, // Azul
        {0.3f, 0.0f, 0.5f}, // Índigo
        {0.5f, 0.0f, 1.0f}  // Violeta
    };


    for (int i = 0; i < segments; ++i) {
        float theta = i * angleStep;
        float nextTheta = (i + 1) * angleStep;

        // Bottom circle
        coordinates.push_back(0.0f);
        coordinates.push_back(0.0f);
        coordinates.push_back(0.0f);

        coordinates.push_back(radius * cos(theta));
        coordinates.push_back(0.0f);
        coordinates.push_back(radius * sin(theta));

        coordinates.push_back(radius * cos(nextTheta));
        coordinates.push_back(0.0f);
        coordinates.push_back(radius * sin(nextTheta));

        // Top circle
        coordinates.push_back(0.0f);
        coordinates.push_back(height);
        coordinates.push_back(0.0f);

        coordinates.push_back(radius * cos(theta));
        coordinates.push_back(height);
        coordinates.push_back(radius * sin(theta));

        coordinates.push_back(radius * cos(nextTheta));
        coordinates.push_back(height);
        coordinates.push_back(radius * sin(nextTheta));

        // Asignar colores del arcoíris a cada cara lateral
        const auto& color = rainbow_colors[i % rainbow_colors.size()];
        for (int j = 0; j < 6; ++j) {
            colors.push_back(color[0]); // R
            colors.push_back(color[1]); // G
            colors.push_back(color[2]); // B
        }

        // Indices for sides (reverse winding order)
        indices.push_back(i * 6 + 4); // Triangle 1
        indices.push_back(i * 6 + 2);
        indices.push_back(i * 6 + 1);

        indices.push_back(i * 6 + 4); // Triangle 2
        indices.push_back(i * 6 + 5);
        indices.push_back(i * 6 + 2);
    }

    // Correct winding for bottom and top caps
    for (int i = 0; i < segments; ++i) {
        int current = i * 6;
        int next = ((i + 1) % segments) * 6;

        // Bottom cap (clockwise)
        indices.push_back(next + 1);
        indices.push_back(current);
        indices.push_back(current + 1);

        // Top cap (anticlockwise)
        indices.push_back(current + 4);
        indices.push_back(current + 3);
        indices.push_back(next + 4);
    }
}

void Cylinder::render() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
