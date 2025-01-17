///
/// Scene.hpp
///

#ifndef SCENE_HPP
#define SCENE_HPP

#include "Cube.hpp"
#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm.hpp>

namespace udit {

    class Scene {
    private:
        static const std::string vertex_shader_code; // C�digo fuente del shader de v�rtices
        static const std::string fragment_shader_code; // C�digo fuente del shader de fragmentos

        GLint model_view_matrix_id; // Identificador de la matriz de modelo-vista en el shader
        GLint projection_matrix_id; // Identificador de la matriz de proyecci�n en el shader

        std::vector<Cube> cubes; // Arreglo de cubos
        std::vector<glm::vec3> cube_positions; // Posiciones de los cubos

        float angle; // �ngulo para las transformaciones animadas

        GLuint compile_shaders(); // M�todo para compilar los shaders
        void show_compilation_error(GLuint shader_id); // Maneja errores de compilaci�n de shaders
        void show_linkage_error(GLuint program_id); // Maneja errores de enlace de programas de shaders

    public:
        Scene(int width, int height); // Constructor

        void update(); // Actualiza las animaciones y estados
        void render(); // Renderiza la escena
        void resize(int width, int height); // Ajusta la proyecci�n cuando se redimensiona la ventana
    };

}

#endif

