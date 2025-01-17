///
/// Scene.cpp
///

#include "Scene.hpp"

#include <iostream>
#include <cassert>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace udit {

    using namespace std;

    const std::string Scene::vertex_shader_flat =
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

    const std::string Scene::fragment_shader_flat =
        "#version 330\n"
        "in vec3 front_color;\n"
        "out vec4 fragment_color;\n"
        "void main()\n"
        "{\n"
        "    fragment_color = vec4(front_color, 1.0);\n"
        "}";

    const std::string Scene::vertex_shader_gouraud =
        "#version 330\n"
        "uniform mat4 model_view_matrix;\n"
        "uniform mat4 projection_matrix;\n"
        "layout (location = 0) in vec3 vertex_coordinates;\n"
        "layout (location = 1) in vec3 vertex_normal;\n"
        "out vec3 front_color;\n"
        "void main()\n"
        "{\n"
        "    vec3 light_position = vec3(5.0, 5.0, 5.0);\n"
        "    vec3 light_color = vec3(1.0, 1.0, 1.0);\n"
        "    vec3 ambient = vec3(0.2, 0.2, 0.2);\n"
        "    vec3 transformed_vertex = vec3(model_view_matrix * vec4(vertex_coordinates, 1.0));\n"
        "    vec3 normal = normalize(mat3(model_view_matrix) * vertex_normal);\n"
        "    vec3 light_dir = normalize(light_position - transformed_vertex);\n"
        "    float diffuse_intensity = max(dot(normal, light_dir), 0.0);\n"
        "    vec3 diffuse = diffuse_intensity * light_color;\n"
        "    front_color = ambient + diffuse;\n"
        "    gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);\n"
        "}";

    const std::string Scene::fragment_shader_gouraud =
        "#version 330\n"
        "in vec3 front_color;\n"
        "out vec4 fragment_color;\n"
        "void main()\n"
        "{\n"
        "    fragment_color = vec4(front_color, 1.0);\n"
        "}";

    Scene::Scene(int width, int height) : angle(0.0f) {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        shader_flat = compile_shaders(vertex_shader_flat, fragment_shader_flat);
        shader_gouraud = compile_shaders(vertex_shader_gouraud, fragment_shader_gouraud);

        resize(width, height);
    }

    void Scene::update() {
        angle += 0.01f;
    }

    void Scene::render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model_view_matrix(1.0f);

        // Renderizar con shader sin iluminación
        glUseProgram(shader_flat);
        model_view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -6.0f));
        glUniformMatrix4fv(model_view_matrix_id_flat, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
        plane.render();

        model_view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0.0f, -6.0f));
        glUniformMatrix4fv(model_view_matrix_id_flat, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
        cylinder.render();

        model_view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, -6.0f));
        glUniformMatrix4fv(model_view_matrix_id_flat, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
        cone.render();

        // Renderizar con iluminación Gouraud
        glUseProgram(shader_gouraud);
        model_view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, -6.0f));
        glUniformMatrix4fv(model_view_matrix_id_gouraud, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
        cube.render();
    }

    void Scene::resize(int width, int height) {
        glm::mat4 projection_matrix = glm::perspective(45.0f, (float)width / height, 0.1f, 100.0f);
        glUniformMatrix4fv(projection_matrix_id_flat, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        glUniformMatrix4fv(projection_matrix_id_gouraud, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        glViewport(0, 0, width, height);
    }

    GLuint Scene::compile_shaders(const std::string& vertex_shader, const std::string& fragment_shader) {
        GLint succeeded = GL_FALSE;

        GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

        const char* vertex_code = vertex_shader.c_str();
        const char* fragment_code = fragment_shader.c_str();

        glShaderSource(vertex_shader_id, 1, &vertex_code, nullptr);
        glShaderSource(fragment_shader_id, 1, &fragment_code, nullptr);

        glCompileShader(vertex_shader_id);
        glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(vertex_shader_id);

        glCompileShader(fragment_shader_id);
        glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(fragment_shader_id);

        GLuint program_id = glCreateProgram();
        glAttachShader(program_id, vertex_shader_id);
        glAttachShader(program_id, fragment_shader_id);

        glLinkProgram(program_id);
        glGetProgramiv(program_id, GL_LINK_STATUS, &succeeded);
        if (!succeeded) show_linkage_error(program_id);

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);

        return program_id;
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

