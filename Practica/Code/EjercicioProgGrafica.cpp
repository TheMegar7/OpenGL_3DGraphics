// 
// Codigo por raúl fabregas libre de uso
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
        "Practica OpenGL",
        Window::Position::CENTERED,
        Window::Position::CENTERED,
        viewport_width,
        viewport_height,
        { 3, 3 }
    );

    Scene scene(viewport_width, viewport_height);

    bool exit = false;
    int  mouse_x = 0;
    int  mouse_y = 0;
    bool button_down = false;

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

            switch (event.type)
            {
                case SDL_MOUSEBUTTONDOWN: { //Se añade un bloque `{ }` aquí
                    int buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
                    bool left_down = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);

                    if (left_down && !button_down)
                        scene.on_click(mouse_x, mouse_y, button_down = true);
                    break;
                }
                case SDL_MOUSEBUTTONUP: { //Se añade un bloque `{ }` aquí
                    int buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
                    bool left_down = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);

                    if (!left_down && button_down)
                        scene.on_click(mouse_x, mouse_y, button_down = false);
                    break;
                }
                case SDL_MOUSEMOTION: { //Se añade un bloque `{ }` aquí
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    scene.on_drag(mouse_x, mouse_y);
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