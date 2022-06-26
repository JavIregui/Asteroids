/*
 * INTRO SCENE
 * Copyright © 2022 Javier Iregui Conde
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * iregui.javi@gmail.com
 */

#include "Intro_Scene.hpp"
#include "Menu_Scene.hpp"
#include <basics/Canvas>
#include <basics/Director>

using namespace basics;
using namespace std;

namespace example
{

    // ---------------------------------------------------------------------------------------------

    /// Configura la escena al abrirse
    bool Intro_Scene::initialize ()
    {
        if (state == UNINITIALIZED)
        {
            state = LOADING;
        }
        else
        {
            timer.reset ();

            opacity = 0.f;
            sceneOver = false;

            state   = FADING_IN;
        }
        return true;
    }

    // ---------------------------------------------------------------------------------------------

    /// Actualiza la escena dependiendo de su estado
    void Intro_Scene::update (float time)
    {
        if (!suspended) switch (state)
            {
                case LOADING:    update_loading    (); break;
                case FADING_IN:  update_fading_in  (); break;
                case WAITING:    update_waiting    (); break;
                case FADING_OUT: update_fading_out (); break;
                case FINISHED: director.run_scene (shared_ptr< Scene >(new Menu_Scene)); break;
                default: break;
            }
    }

    // ---------------------------------------------------------------------------------------------

    /// Dibuja los logos por orden y cambia de opacidad
    void Intro_Scene::render (Graphics_Context::Accessor & context)
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

                if (esne_texture && logo_texture)
                {
                    canvas->set_opacity (opacity);

                    switch (sceneOver) {
                        case true:
                            canvas->fill_rectangle(
                                    { canvas_width * .5f, canvas_height * .5f },
                                    { logo_texture->get_width (), logo_texture->get_height () },
                                    logo_texture. get ()
                            );
                            break;
                        case false:
                            canvas->fill_rectangle(
                                    { canvas_width * .5f, canvas_height * .5f },
                                    { esne_texture->get_width (), esne_texture->get_height () },
                                    esne_texture. get ()
                                    );
                            break;
                    }
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------

    /// Carga las texturas
    void Intro_Scene::update_loading ()
    {
        Graphics_Context::Accessor context = director.lock_graphics_context ();

        if (context)
        {
            // Se cargan las texturas
            esne_texture = Texture_2D::create (0, context, "esne.png");
            logo_texture = Texture_2D::create (0, context, "asteroids.png");

            // Se comprueba si las texturas se han podido cargar correctamente:
            if (esne_texture && logo_texture)
            {
                context->add (esne_texture);
                context->add (logo_texture);

                timer.reset ();

                opacity = 0.f;
                sceneOver = 0;
                state   = FADING_IN;
            }
            else
                state   = ERROR;
        }
    }

    /// Cambia el valor de la variable opacidad para crear el efecto del Fade In
    void Intro_Scene::update_fading_in ()
    {
        float elapsed_seconds = timer.get_elapsed_seconds ();

        if (elapsed_seconds < .5f)
        {
            opacity = elapsed_seconds * 2.f;      // Se aumenta la opacidad del logo a medida que pasa el tiempo
        }
        else
        {
            timer.reset ();

            opacity = 1.f;
            state   = WAITING;
        }
    }

    /// Espera un tiempo hasta comenzar el Fade Out
    void Intro_Scene::update_waiting ()
    {
        // Se esperan dos segundos sin hacer nada:
        float elapsed_seconds = timer.get_elapsed_seconds ();

        if (elapsed_seconds > 2.f)
        {
            timer.reset ();

            state = FADING_OUT;
        }
    }

    /// Cambia el valor de la variable opacidad para crear el efecto del Fade Out y pasa al siguiente
    /// logo o indica que la escena ha terminado
    void Intro_Scene::update_fading_out ()
    {
        float elapsed_seconds = timer.get_elapsed_seconds ();

        if (elapsed_seconds < .5f)
        {
            opacity = 1.f - elapsed_seconds * 2.f;      // Se reduce la opacidad de 1 a 0 en medio segundo
        }
        else
        {
            switch (sceneOver) {
                case true:
                    state = FINISHED;
                    break;
                case false:
                    opacity = 0.f;
                    timer.reset ();
                    sceneOver = true;
                    state = FADING_IN;
                    break;

            }
        }
    }

}