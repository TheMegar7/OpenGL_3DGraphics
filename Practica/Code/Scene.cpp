///
/// Scene.cpp
///

#include "Scene.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <gtc/type_ptr.hpp>                 // value_ptr

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
namespace udit {

    using namespace std;
    using namespace glm;

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

    Scene::Scene(int width, int height) : angle(0.0f), cylinder(0.5f, 1.0f, 36), cone(0.5f, 1.0f, 36), camera_position(0.0f, 0.0f, 0.0f), skybox("../../../shared/assets/sky-cube-map-") {
        
        // Habilitar características de OpenGL para ocultar caras traseras y manejar profundidad
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // Color de fondo de la ventana

        GLuint program_id = compile_shaders();
        glUseProgram(program_id);

        model_view_matrix_id = glGetUniformLocation(program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation(program_id, "projection_matrix");

        
        resize(width, height); // Configurar la proyección inicial

        load_mesh("../../../shared/assets/stanford-bunny.obj");

        angle_around_x = angle_delta_x = 0.0;
        angle_around_y = angle_delta_y = 0.0;
        pointer_pressed = false;
    }

    Scene::~Scene()
    {
        glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers(VBO_COUNT, vbo_ids);
    }

    void Scene::update() {
        if (pointer_pressed) // Solo girar si el usuario sigue moviendo el ratón
        {
            angle_around_x += angle_delta_x;
            angle_around_y += angle_delta_y;
        }

        if (angle_around_x < -1.5f) angle_around_x = -1.5f;
        if (angle_around_x > 1.5f) angle_around_x = 1.5f;

        glm::mat4 camera_rotation(1);

        camera_rotation = glm::rotate(camera_rotation, angle_around_y, glm::vec3(0.f, 1.f, 0.f));
        camera_rotation = glm::rotate(camera_rotation, angle_around_x, glm::vec3(1.f, 0.f, 0.f));

        camera.set_target(0, 0, -1);
        camera.rotate(camera_rotation);

        angle += 0.01f; // Incrementar el ángulo para animaciones
    }

    void Scene::render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        glm::mat4 view_matrix = camera.get_transform_matrix_inverse(); // Obtener la vista de la cámara

        glm::mat4 model_view_matrix = view_matrix;

        model_view_matrix = glm::translate(model_view_matrix, glm::vec3(-1.f, 1.0f, -6.0f));
        model_view_matrix = glm::rotate(model_view_matrix, angle, glm::vec3(0.f, 1.f, 0.f));

        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));

        // Se dibuja la malla:

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_SHORT, 0);

        //Segundo conejo
        model_view_matrix = view_matrix;

        model_view_matrix = glm::translate(model_view_matrix, glm::vec3(-1.f, -1.0f, -6.0f));
        model_view_matrix = glm::rotate(model_view_matrix, angle, glm::vec3(0.f, 1.f, 0.f));

        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));

        // Se dibuja la malla:

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_SHORT, 0);

        // Renderizar el plano
        glm::mat4 object_matrix = glm::translate(view_matrix, glm::vec3(-1.0f, 0.0f, -6.0f));
        object_matrix = glm::rotate(object_matrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(object_matrix));
        plane.render();

        // Renderizar el cilindro
        object_matrix = glm::translate(view_matrix, glm::vec3(1.5f, 0.0f, -6.0f));
        object_matrix = glm::rotate(object_matrix, angle, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(object_matrix));
        cylinder.render();

        // Renderizar el cono
        object_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 2.0f, -6.0f));
        object_matrix = glm::rotate(object_matrix, angle, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(object_matrix));
        cone.render();

        //skybox.render(camera);
    }

    void Scene::resize(int width, int height) {
        // Configurar la matriz de proyección en función de las dimensiones de la ventana
        glm::mat4 projection_matrix = glm::perspective(45.0f, (float)width / height, 0.1f, 100.0f);
        glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        glViewport(0, 0, width, height); // Ajustar la vista
    }

    void Scene::move_camera(char direction) {
        float speed = 0.1f;
        glm::vec3 forward(0.0f, 0.0f, -1.0f);
        glm::vec3 right(1.0f, 0.0f, 0.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f); 
        //forward = glm::normalize(glm::vec3(camera.get_target()) - glm::vec3(camera.get_location())); // Dirección en la que apunta la cámara
        //right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f))); // Vector perpendicular para derecha de la cámara
        //up = glm::normalize(glm::cross(forward, glm::vec3(1.0f, 0.0f, 0.0f))); //Lo mismo

        switch (direction) {
        case 'w': camera.move(forward * speed); break;  //Mover en la dirección en la que apunta la cámara
        case 's': camera.move(-forward * speed); break; //Retroceder
        case 'a': camera.move(-right * speed); break;   //Izquierda
        case 'd': camera.move(right * speed); break;    //Derecha
        case 'l': camera.move(up * speed); break;       //Subir (LShift)
        case 'c': camera.move(-up * speed); break;      //Bajar (LCtrl)
        }
    }

    void Scene::on_drag(int pointer_x, int pointer_y)
    {
        if (pointer_pressed)
        {
            float sensitivity = 0.002f; // Reducimos la sensibilidad
            angle_delta_x = glm::radians(sensitivity * float(last_pointer_y - pointer_y));
            angle_delta_y = glm::radians(sensitivity * float(last_pointer_x - pointer_x));

            // Limitar la rotación en el eje X para evitar voltear la cámara
            if (angle_around_x + angle_delta_x < -1.5f)
                angle_delta_x = -1.5f - angle_around_x;
            else if (angle_around_x + angle_delta_x > 1.5f)
                angle_delta_x = 1.5f - angle_around_x;
        }
    }


    void Scene::on_click(int pointer_x, int pointer_y, bool down)
    {
        if ((pointer_pressed = down) == true)
        {
            last_pointer_x = pointer_x;
            last_pointer_y = pointer_y;
        }
        else
        {
            angle_delta_x = angle_delta_y = 0.0;
        }
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

    void Scene::load_mesh(const std::string& mesh_file_path)
    {
        Assimp::Importer importer;

        auto scene = importer.ReadFile
        (
            mesh_file_path,
            aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
        );

        // Si scene es un puntero nulo significa que el archivo no se pudo cargar con éxito:

        if (scene && scene->mNumMeshes > 0)
        {
            // Para este ejemplo se coge la primera malla solamente:

            auto mesh = scene->mMeshes[0];

            size_t number_of_vertices = mesh->mNumVertices;

            // Se generan índices para los VBOs del cubo:

            glGenBuffers(VBO_COUNT, vbo_ids);
            glGenVertexArrays(1, &vao_id);

            // Se activa el VAO del cubo para configurarlo:

            glBindVertexArray(vao_id);

            // Se suben a un VBO los datos de coordenadas y se vinculan al VAO:

            static_assert(sizeof(aiVector3D) == sizeof(fvec3), "aiVector3D should composed of three floats");

            glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
            glBufferData(GL_ARRAY_BUFFER, number_of_vertices * sizeof(aiVector3D), mesh->mVertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            // El archivo del modelo 3D de ejemplo no guarda un color por cada vértice, por lo que se va
            // a crear un array de colores aleatorios (tantos como vértices):

            vector< vec3 > vertex_colors(number_of_vertices);

            for (auto& color : vertex_colors)
            {
                color = random_color();
            }

            // Se suben a un VBO los datos de color y se vinculan al VAO:

            glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
            glBufferData(GL_ARRAY_BUFFER, vertex_colors.size() * sizeof(vec3), vertex_colors.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

            // Los índices en ASSIMP están repartidos en "faces", pero OpenGL necesita un array de enteros
            // por lo que vamos a mover los índices de las "faces" a un array de enteros:

            // Se asume que todas las "faces" son triángulos (revisar el flag aiProcess_Triangulate arriba).

            number_of_indices = mesh->mNumFaces * 3;

            vector< GLshort > indices(number_of_indices);

            auto vertex_index = indices.begin();

            for (unsigned i = 0; i < mesh->mNumFaces; ++i)
            {
                auto& face = mesh->mFaces[i];

                assert(face.mNumIndices == 3);

                *vertex_index++ = face.mIndices[0];
                *vertex_index++ = face.mIndices[1];
                *vertex_index++ = face.mIndices[2];
            }

            // Se suben a un EBO los datos de índices:

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLshort), indices.data(), GL_STATIC_DRAW);
        }
    }

    vec3 Scene::random_color()
    {
        return vec3
        (
            float(rand()) / float(RAND_MAX),
            float(rand()) / float(RAND_MAX),
            float(rand()) / float(RAND_MAX)
        );
    }
}