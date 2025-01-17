///
/// Scene.hpp
///

#ifndef SCENE_HPP
#define SCENE_HPP

#include "Plane.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"
#include <string>
#include <glad/glad.h>
#include <glm.hpp>

namespace udit {

    class Scene {
    private:
        static const std::string vertex_shader_code; // Código fuente del shader de vértices
        static const std::string fragment_shader_code; // Código fuente del shader de fragmentos

        GLint model_view_matrix_id; // Identificador de la matriz de modelo-vista en el shader
        GLint projection_matrix_id; // Identificador de la matriz de proyección en el shader

        Plane plane; // Instancia del plano
        Cylinder cylinder; // Instancia del cilindro
        Cone cone; // Instancia del cono

        float angle; // Ángulo para las transformaciones animadas

        glm::mat4 view_matrix; // Matriz de vista para la cámara
        glm::vec3 camera_position; // Posición de la cámara

        GLuint compile_shaders(); // Método para compilar los shaders
        void show_compilation_error(GLuint shader_id); // Maneja errores de compilación de shaders
        void show_linkage_error(GLuint program_id); // Maneja errores de enlace de programas de shaders

    public:
        Scene(int width, int height); // Constructor

        void update(); // Actualiza las animaciones y estados
        void render(); // Renderiza la escena
        void resize(int width, int height); // Ajusta la proyección cuando se redimensiona la ventana
        void move_camera(char direction); // Método para mover la cámara
    };

}

#endif