///
/// Scene.cpp
///

#include "Scene.hpp"

#include <iostream>
#include <cassert>

#include <glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <gtc/type_ptr.hpp>                 // value_ptr

namespace udit {

    using namespace std;

    const std::string Scene::vertex_shader_code =
        "#version 330\n"
        "uniform mat4 model_view_matrix;\n"
        "uniform mat4 projection_matrix;\n"
        "layout (location = 0) in vec3 vertex_coordinates;\n"
        "layout (location = 1) in vec3 vertex_color;\n"
        "out vec3 front_color;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);\n"
        "    front_color = vertex_color;\n"
        "}";

    const std::string Scene::fragment_shader_code =
        "#version 330\n"
        "in vec3 front_color;\n"
        "out vec4 fragment_color;\n"
        "void main()\n"
        "{\n"
        "    fragment_color = vec4(front_color, 1.0);\n"
        "}";

    Scene::Scene(int width, int height) : angle(0.0f), cylinder(0.5f, 1.0f, 36), cone(0.5f, 1.0f, 36) {
        // Habilitar características de OpenGL para ocultar caras traseras y manejar profundidad
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Color de fondo de la ventana

        GLuint program_id = compile_shaders();
        glUseProgram(program_id);

        model_view_matrix_id = glGetUniformLocation(program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation(program_id, "projection_matrix");

        
        resize(width, height); // Configurar la proyección inicial
    }

    void Scene::update() {
        angle += 0.01f; // Incrementar el ángulo para animaciones
    }

    void Scene::render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpiar el búfer de color y profundidad

        glm::mat4 model_view_matrix(1.0f); // Matriz identidad

        // Transformar y renderizar el plano
        model_view_matrix = glm::translate(model_view_matrix, glm::vec3(-1.0f, 0.0f, -6.0f));
        model_view_matrix = glm::rotate(model_view_matrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
        plane.render();

        // Transformar y renderizar el cilindro
        model_view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, -6.0f));
        model_view_matrix = glm::rotate(model_view_matrix, angle, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
        cylinder.render();

        // Transformar y renderizar el cono
        model_view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -6.0f));
        model_view_matrix = glm::rotate(model_view_matrix, angle, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
        cone.render();
    }

    void Scene::resize(int width, int height) {
        // Configurar la matriz de proyección en función de las dimensiones de la ventana
        glm::mat4 projection_matrix = glm::perspective(45.0f, (float)width / height, 0.1f, 100.0f);
        glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        glViewport(0, 0, width, height); // Ajustar la vista
    }

    GLuint Scene::compile_shaders() {
        GLint succeeded = GL_FALSE;

        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

        const char* vertex_code = vertex_shader_code.c_str();
        const char* fragment_code = fragment_shader_code.c_str();

        glShaderSource(vertex_shader, 1, &vertex_code, nullptr);
        glShaderSource(fragment_shader, 1, &fragment_code, nullptr);

        glCompileShader(vertex_shader);
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(vertex_shader);

        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(fragment_shader);

        GLuint program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);

        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &succeeded);
        if (!succeeded) show_linkage_error(program);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return program;
    }

    void Scene::show_compilation_error(GLuint shader_id) {
        GLint log_length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

        std::vector<char> log(log_length);
        glGetShaderInfoLog(shader_id, log_length, nullptr, log.data());

        std::cerr << "Shader Compilation Error:\n" << log.data() << std::endl;
    }

    void Scene::show_linkage_error(GLuint program_id) {
        GLint log_length;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        std::vector<char> log(log_length);
        glGetProgramInfoLog(program_id, log_length, nullptr, log.data());

        std::cerr << "Program Linkage Error:\n" << log.data() << std::endl;
    }

}