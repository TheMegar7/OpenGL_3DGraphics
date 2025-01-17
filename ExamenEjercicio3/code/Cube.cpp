///
/// Cube.cpp
///

#include "Cube.hpp"

namespace udit {

    const GLfloat Cube::coordinates[] = {
        // Frente
       -1.0f, -1.0f, +1.0f,
       +1.0f, -1.0f, +1.0f,
       +1.0f, +1.0f, +1.0f,
       -1.0f, +1.0f, +1.0f,
       // Derecha
      +1.0f, -1.0f, +1.0f,
      +1.0f, -1.0f, -1.0f,
      +1.0f, +1.0f, -1.0f,
      +1.0f, +1.0f, +1.0f,
      // Atrás
     +1.0f, -1.0f, -1.0f,
     -1.0f, -1.0f, -1.0f,
     -1.0f, +1.0f, -1.0f,
     +1.0f, +1.0f, -1.0f,
     // Izquierda
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, +1.0f,
    -1.0f, +1.0f, +1.0f,
    -1.0f, +1.0f, -1.0f,
    // Superior
   -1.0f, +1.0f, +1.0f,
   +1.0f, +1.0f, +1.0f,
   +1.0f, +1.0f, -1.0f,
   -1.0f, +1.0f, -1.0f,
   // Inferior
  -1.0f, -1.0f, -1.0f,
  +1.0f, -1.0f, -1.0f,
  +1.0f, -1.0f, +1.0f,
  -1.0f, -1.0f, +1.0f
    };

    const GLfloat Cube::colors[] = {
        // Frente (rojo)
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        // Derecha (amarillo)
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        // Atrás (verde)
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        // Izquierda (azul)
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        // Superior (blanco)
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        // Inferior (naranja)
        1.0f, 0.5f, 0.0f,
        1.0f, 0.5f, 0.0f,
        1.0f, 0.5f, 0.0f,
        1.0f, 0.5f, 0.0f
    };

    const GLubyte Cube::indices[] = {
        0, 1, 2,  0, 2, 3, // Frente
        4, 5, 6,  4, 6, 7, // Derecha
        8, 9, 10, 8, 10, 11, // Atrás
        12, 13, 14, 12, 14, 15, // Izquierda
        16, 17, 18, 16, 18, 19, // Superior
        20, 21, 22, 20, 22, 23  // Inferior
    };

    Cube::Cube() {
        glGenBuffers(VBO_COUNT, vbo_ids);
        glGenVertexArrays(1, &vao_id);

        glBindVertexArray(vao_id);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    Cube::~Cube() {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    void Cube::render() {
        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_BYTE, 0);
        glBindVertexArray(0);
    }

}
