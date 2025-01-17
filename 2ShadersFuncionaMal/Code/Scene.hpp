///
/// Scene.hpp
///

#ifndef SCENE_HPP
#define SCENE_HPP

#include "Plane.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"
#include "Cube.hpp"
#include <glad/glad.h>
#include <glm.hpp>
#include <string>

namespace udit {

    class Scene {
    private:
        static const std::string vertex_shader_flat;
        static const std::string fragment_shader_flat;
        static const std::string vertex_shader_gouraud;
        static const std::string fragment_shader_gouraud;

        GLuint shader_flat;
        GLuint shader_gouraud;

        GLint model_view_matrix_id_flat;
        GLint projection_matrix_id_flat;
        GLint model_view_matrix_id_gouraud;
        GLint projection_matrix_id_gouraud;

        Plane plane;
        Cylinder cylinder;
        Cone cone;
        Cube cube;

        float angle;

        GLuint compile_shaders(const std::string& vertex_shader, const std::string& fragment_shader);
        void show_compilation_error(GLuint shader_id);
        void show_linkage_error(GLuint program_id);

    public:
        Scene(int width, int height);
        void update();
        void render();
        void resize(int width, int height);
    };

}

#endif