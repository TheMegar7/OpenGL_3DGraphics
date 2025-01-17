///
/// Scene.hpp
///

#ifndef SCENE_HPP
#define SCENE_HPP

#include "Camera.hpp"
#include "Skybox.hpp"

#include "Plane.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"
#include <string>
#include <glad/glad.h>
#include <glm.hpp>

namespace udit {

    class Scene {
    private:
        #pragma region Camera and skybox
        Camera camera;
        Skybox skybox;

        int    width;
        int    height;

        float  angle_around_x;
        float  angle_around_y;
        float  angle_delta_x;
        float  angle_delta_y;

        bool   pointer_pressed;
        int    last_pointer_x;
        int    last_pointer_y;
        #pragma endregion

        enum
        {
            COORDINATES_VBO,
            COLORS_VBO,
            INDICES_EBO,
            VBO_COUNT
        };

        static const std::string vertex_shader_code; // C�digo fuente del shader de v�rtices
        static const std::string fragment_shader_code; // C�digo fuente del shader de fragmentos

        GLuint  vbo_ids[VBO_COUNT];
        GLuint  vao_id;

        GLsizei number_of_indices;

        GLint model_view_matrix_id; // Identificador de la matriz de modelo-vista en el shader
        GLint projection_matrix_id; // Identificador de la matriz de proyecci�n en el shader

        Plane plane; // Instancia del plano
        Cylinder cylinder; // Instancia del cilindro
        Cone cone; // Instancia del cono

        float angle; // �ngulo para las transformaciones animadas

        glm::mat4 view_matrix; // Matriz de vista para la c�mara
        glm::vec3 camera_position; // Posici�n de la c�mara

        GLuint compile_shaders(); // M�todo para compilar los shaders
        void show_compilation_error(GLuint shader_id); // Maneja errores de compilaci�n de shaders
        void show_linkage_error(GLuint program_id); // Maneja errores de enlace de programas de shaders

    public:
        Scene(int width, int height); // Constructor
        ~Scene();

        void update(); // Actualiza las animaciones y estados
        void render(); // Renderiza la escena
        void resize(int width, int height); // Ajusta la proyecci�n cuando se redimensiona la ventana
        void move_camera(char direction); // M�todo para mover la c�mara
        void on_drag(int pointer_x, int pointer_y);
        void on_click(int pointer_x, int pointer_y, bool down);

        void load_mesh(const std::string& mesh_file_path);
        glm::vec3 random_color();
    };

}

#endif