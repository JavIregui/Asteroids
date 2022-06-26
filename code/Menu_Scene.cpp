/*
 * MENU SCENE
 * Copyright © 2022 Javier Iregui Conde
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * iregui.javi@gmail.com
 */

#include "Menu_Scene.hpp"
#include "Game_Scene.hpp"
#include <basics/Canvas>
#include <basics/Director>
#include <basics/Transformation>

using namespace basics;
using namespace std;

namespace example
{

    Menu_Scene::Menu_Scene()
    {
        state         = LOADING;
        screen        = MENU;
        suspended     = true;
        canvas_width  = 1280;
        canvas_height =  720;
    }

    // ---------------------------------------------------------------------------------------------

    bool Menu_Scene::initialize ()
    {
        for (auto & option : options)
        {
            option.is_pressed = false;
        }

        return true;
    }

    // ---------------------------------------------------------------------------------------------

    void Menu_Scene::handle (basics::Event & event)
    {
        if (state == READY)                     // Se descartan los eventos cuando la escena está LOADING
        {
            switch (event.id)
            {
                case ID(touch-started):         // El usuario toca la pantalla
                case ID(touch-moved):
                {
                    // Se determina qué opción se ha tocado:

                    Point2f touch_location = { *event[ID(x)].as< var::Float > (), *event[ID(y)].as< var::Float > () };
                    int     option_touched = option_at (touch_location);

                    // Solo se puede tocar una opción a la vez (para evitar selecciones múltiples),
                    // por lo que solo una se considera presionada (el resto se "sueltan"):

                    for (int index = 0; index < number_of_options; ++index)
                    {
                        options[index].is_pressed = index == option_touched;
                    }

                    break;
                }

                case ID(touch-ended):           // El usuario deja de tocar la pantalla
                {
                    // Se "sueltan" todas las opciones:
                    for (auto & option : options) option.is_pressed = false;

                    // Se determina qué opción se ha dejado de tocar la última y se actúa como corresponda:
                    Point2f touch_location = { *event[ID(x)].as< var::Float > (), *event[ID(y)].as< var::Float > () };

                    switch (screen) {
                        case MENU:
                            if (option_at (touch_location) == JUGAR)
                            {
                                director.run_scene (shared_ptr< Scene >(new Game_Scene));
                            }
                            else if (option_at (touch_location) == INSTRUCCIONES){
                                screen = INSTRUCCIONES_TEXTO;
                            }
                            else if (option_at (touch_location) == CREDITOS){
                                screen = CREDITOS_TEXTO;
                            }
                            break;

                        case INSTRUCCIONES_TEXTO:
                            screen = MENU;
                            break;

                        case CREDITOS_TEXTO:
                            screen = MENU;
                            break;
                    }
                    break;
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Menu_Scene::update (float time)
    {
        if (!suspended) if (state == LOADING)
        {
            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                jugar_texture = Texture_2D::create (0, context, "menu-scene/jugar.png");
                logo_texture = Texture_2D::create (0, context, "asteroids.png");
                instrucciones_texture = Texture_2D::create (0, context, "menu-scene/instrucciones.png");
                creditos_texture = Texture_2D::create (0, context, "menu-scene/creditos.png");
                instrucciones_texto = Texture_2D::create (0, context, "menu-scene/textoInstrucciones.png");
                creditos_texto = Texture_2D::create (0, context, "menu-scene/textoCreditos.png");

                if (logo_texture && jugar_texture && instrucciones_texture && creditos_texture && instrucciones_texto && creditos_texto)
                {
                    context->add (logo_texture);
                    context->add (jugar_texture);
                    context->add (instrucciones_texture);
                    context->add (creditos_texture);
                    context->add (instrucciones_texto);
                    context->add (creditos_texto);
                    state = READY;
                }
                else
                {
                    state = ERROR;
                }

                if (state == READY)
                {
                    configure_options();
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Menu_Scene::render (Graphics_Context::Accessor & context)
    {
        if (!suspended)
        {
            // El canvas se puede haber creado previamente, en cuyo caso solo hay que pedirlo:

            Canvas * canvas = context->get_renderer< Canvas > (ID(canvas));

            // Si no se ha creado previamente, hay que crearlo una vez:

            if (!canvas)
            {
                 canvas = Canvas::create (ID(canvas), context, {{ canvas_width, canvas_height }});
            }

            // Si el canvas se ha podido obtener o crear, se puede dibujar con él:

            if (canvas)
            {
                canvas->clear ();

                if (state == READY)
                {
                    if (logo_texture && jugar_texture && instrucciones_texture && creditos_texture && instrucciones_texto && creditos_texto){
                        switch (screen) {
                            case MENU:
                                canvas->fill_rectangle
                                        (
                                                { canvas_width * .5f, canvas_height * .75f },
                                                { (logo_texture->get_width ())*1.f, (logo_texture->get_height ())*1.f },
                                                logo_texture. get ()
                                        );
                                canvas->fill_rectangle
                                        (
                                                { options[0].position[0], options[0].position[1] },
                                                { (jugar_texture->get_width ()), (jugar_texture->get_height ()) },
                                                jugar_texture. get ()
                                        );
                                canvas->fill_rectangle
                                        (
                                                { options[1].position[0], options[1].position[1] },
                                                { (instrucciones_texture->get_width ()), (instrucciones_texture->get_height ()) },
                                                instrucciones_texture. get ()
                                        );
                                canvas->fill_rectangle
                                        (
                                                { options[2].position[0], options[2].position[1] },
                                                { (creditos_texture->get_width ()), (creditos_texture->get_height ()) },
                                                creditos_texture. get ()
                                        );
                                break;

                            case INSTRUCCIONES_TEXTO:
                                canvas->fill_rectangle
                                        (
                                                { canvas_width * .5f, canvas_height * .5f },
                                                { (instrucciones_texto->get_width ())*1.f, (instrucciones_texto->get_height ())*1.f },
                                                instrucciones_texto. get ()
                                        );
                                break;

                            case CREDITOS_TEXTO:
                                canvas->fill_rectangle
                                        (
                                                { canvas_width * .5f, canvas_height * .5f },
                                                { (creditos_texto->get_width ())*1.f, (creditos_texto->get_height ())*1.f },
                                                creditos_texto. get ()
                                        );
                                break;
                        }
                    }
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Menu_Scene::configure_options ()
    {
        // Se calcula la altura total del menú:
        float menu_height = 0;

        for (auto & option : options) menu_height += (jugar_texture->get_height ()) * number_of_options;

        // Se calcula la posición del borde superior del menú en su conjunto de modo que
        // quede centrado verticalmente:
        float option_top = canvas_height / 2.f;

        // Se establece la posición del borde superior de cada opción:
        for (unsigned index = 0; index < number_of_options; ++index)
        {
            options[index].position = Point2f{ canvas_width / 2.f, option_top };

            option_top -= (jugar_texture->get_height ());
        }

        // Se restablece la presión de cada opción:
        initialize ();
    }

    // ---------------------------------------------------------------------------------------------

    int Menu_Scene::option_at (const Point2f & point)
    {
        for (int index = 0; index < number_of_options; ++index)
        {
            const Option & option = options[index];

            if
            (
                point[0] > option.position[0] - instrucciones_texture->get_width()  &&
                point[0] < option.position[0] + instrucciones_texture->get_width()  &&
                point[1] > option.position[1] - jugar_texture->get_height() &&
                point[1] < option.position[1] + jugar_texture->get_height()
            )
            {
                return index;
            }
        }

        return -1;
    }

}
