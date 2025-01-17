
///
/// Raul Cube.hpp
///

#ifndef CUBE_HEADER
#define CUBE_HEADER

#include <glad/glad.h>

namespace udit {

    class Cube {
    private:
        enum {
            COORDINATES_VBO,
            COLORS_VBO,
            INDICES_EBO,
            VBO_COUNT
        };

        static const GLfloat coordinates[];
        static const GLfloat colors[];
        static const GLubyte indices[];

        GLuint vbo_ids[VBO_COUNT];
        GLuint vao_id;

    public:
        Cube();
        ~Cube();

        void render();
    };

}

#endif
