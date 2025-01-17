// 
//

#include "Scene.hpp"
#include "Window.hpp"

using udit::Scene;
using udit::Window;

int main(int, char* [])
{
    constexpr unsigned viewport_width = 1024;
    constexpr unsigned viewport_height = 576;

    Window window
    (
        "OpenGL Primitives Example",
        Window::Position::CENTERED,
        Window::Position::CENTERED,
        viewport_width,
        viewport_height,
        { 3, 3 }
    );

    Scene scene(viewport_width, viewport_height);

    bool exit = false;

    do
    {
        // Procesar eventos acumulados:

        SDL_Event event;

        while (SDL_PollEvent(&event) > 0)
        {
            if (event.type == SDL_QUIT)
            {
                exit = true;
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_w:
                    scene.move_camera('w');
                    break;
                case SDLK_s:
                    scene.move_camera('s');
                    break;
                case SDLK_a:
                    scene.move_camera('a');
                    break;
                case SDLK_d:
                    scene.move_camera('d');
                    break;
                case SDLK_LSHIFT: // Subir cámara
                    scene.move_camera('l');
                    break;
                case SDLK_LCTRL:  // Bajar cámara
                    scene.move_camera('c');
                    break;
                }
            }

            
        }

        // Actualizar la escena:

        scene.update();

        // Redibujar la escena:

        scene.render();

        // Actualizar el contenido de la ventana:

        window.swap_buffers();
    } while (!exit);

    SDL_Quit();

    return 0;
}