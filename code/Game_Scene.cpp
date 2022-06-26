/*
 * GAME SCENE
 * Copyright © 2022 Javier Iregui Conde
 *
 * Distributed under the Boost Software License, version  1.0
 * See documents/LICENSE.TXT or www.boost.org/LICENSE_1_0.txt
 *
 * iregui.javi@gmail.com
 */

#include "Game_Scene.hpp"
#include "Menu_Scene.hpp"

#include <cstdlib>
#include <basics/Canvas>
#include <basics/Director>

using namespace basics;
using namespace std;

namespace example
{
    // ---------------------------------------------------------------------------------------------
    // ID y ruta de las texturas que se deben cargar para esta escena. La textura con el mensaje de
    // carga está la primera para poder dibujarla cuanto antes:

    Game_Scene::Texture_Data Game_Scene::textures_data[] =
    {
        { ID(loading),"game-scene/cargando.png"},
        { ID(player),"game-scene/player.png"},
        { ID(asteroide),"game-scene/asteroide.png"},
        { ID(bala),"game-scene/bala.png"},
        { ID(pausa),"game-scene/pausa.png"},
        { ID(botonPausa),"game-scene/botonPausa.png"},
        { ID(forward),"game-scene/forward.png"},
        { ID(right),"game-scene/right.png"},
        { ID(left),"game-scene/left.png"},
        { ID(shoot),"game-scene/shoot.png"}

    };

    // Pâra determinar el número de items en el array textures_data, se divide el tamaño en bytes
    // del array completo entre el tamaño en bytes de un item:

    unsigned Game_Scene::textures_count = sizeof(textures_data) / sizeof(Texture_Data);

    // ---------------------------------------------------------------------------------------------

    Game_Scene::Game_Scene()
    {
        // Se establece la resolución virtual (independiente de la resolución virtual del dispositivo).
        // En este caso no se hace ajuste de aspect ratio, por lo que puede haber distorsión cuando
        // el aspect ratio real de la pantalla del dispositivo es distinto.

        canvas_width  = 1280;
        canvas_height =  720;

        // Se inicia la semilla del generador de números aleatorios:

        srand (unsigned(time(nullptr)));

        // Se inicializan otros atributos:

        initialize ();
    }

    // ---------------------------------------------------------------------------------------------
    // Algunos atributos se inicializan en este método en lugar de hacerlo en el constructor porque
    // este método puede ser llamado más veces para restablecer el estado de la escena y el constructor
    // solo se invoca una vez.

    bool Game_Scene::initialize ()
    {
        state     = LOADING;
        suspended = true;
        gameplay  = UNINITIALIZED;

        return true;
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::suspend ()
    {
        suspended = true;               // Se marca que la escena ha pasado a segundo plano
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::resume ()
    {
        suspended = false;              // Se marca que la escena ha pasado a primer plano
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::handle (Event & event)
    {
        if (state == RUNNING)               // Se descartan los eventos cuando la escena está LOADING
        {
            if (gameplay == WAITING_TO_START)
            {
                start_playing ();           // Se empieza a jugar cuando el usuario toca la pantalla por primera vez
            }
            else switch (event.id)
            {
                case ID(touch-started):         // El usuario toca la pantalla
                {
                    if((*event[ID(x)].as< var::Float > ()) > canvas_width * .2f
                       && (*event[ID(x)].as< var::Float > ()) < canvas_width * .2f + 200
                       && (*event[ID(y)].as< var::Float > ()) > canvas_height * .0f
                       && (*event[ID(y)].as< var::Float > ()) < canvas_height * .2f + 100){

                        right = true;

                    }
                    if((*event[ID(x)].as< var::Float > ()) > canvas_width * .0f
                       && (*event[ID(x)].as< var::Float > ()) < canvas_width * .1f + 100
                       && (*event[ID(y)].as< var::Float > ()) > canvas_height * .0f
                       && (*event[ID(y)].as< var::Float > ()) < canvas_height * .2f + 100){

                        left = true;

                    }

                    if((*event[ID(x)].as< var::Float > ()) > canvas_width * .9f
                       && (*event[ID(x)].as< var::Float > ()) < canvas_width * 1.f
                       && (*event[ID(y)].as< var::Float > ()) > canvas_height * .0f
                       && (*event[ID(y)].as< var::Float > ()) < canvas_height * 2.f + 100){

                        move = true;

                    }
                    else if((*event[ID(x)].as< var::Float > ()) > canvas_width * .8f - 100
                       && (*event[ID(x)].as< var::Float > ()) < canvas_width * 8.f - 200
                       && (*event[ID(y)].as< var::Float > ()) > canvas_height * .0f
                       && (*event[ID(y)].as< var::Float > ()) < canvas_height * 2.f + 100){

                        Sprite_Handle newBullet(new Sprite( textures[ID(bala)].get () ));
                        newBullet->set_anchor( CENTER);
                        newBullet->set_position ({ player->get_position_x(), player->get_position_y()});
                        newBullet->set_size({ 5, 5});
                        newBullet->set_speed(Vector2f(cos(angle) * 0 - sin(angle) * 1, sin(angle) * 0 + cos(angle) * 1) * 250);

                        bullets.push_back(newBullet);
                    }

                    break;
                }
                case ID(touch-moved):
                {
                    if((*event[ID(x)].as< var::Float > ()) > canvas_width * .2f
                       && (*event[ID(x)].as< var::Float > ()) < canvas_width * .2f + 200
                       && (*event[ID(y)].as< var::Float > ()) > canvas_height * .0f
                       && (*event[ID(y)].as< var::Float > ()) < canvas_height * .2f + 100){

                        right = true;

                    }
                    if((*event[ID(x)].as< var::Float > ()) > canvas_width * .0f
                       && (*event[ID(x)].as< var::Float > ()) < canvas_width * .1f + 100
                       && (*event[ID(y)].as< var::Float > ()) > canvas_height * .0f
                       && (*event[ID(y)].as< var::Float > ()) < canvas_height * .2f + 100){

                        left = true;

                    }


                    if((*event[ID(x)].as< var::Float > ()) > canvas_width * .9f
                       && (*event[ID(x)].as< var::Float > ()) < canvas_width * 1.f
                       && (*event[ID(y)].as< var::Float > ()) > canvas_height * .0f
                       && (*event[ID(y)].as< var::Float > ()) < canvas_height * .2f + 100){

                        move = true;

                    }
                    break;
                }

                case ID(touch-ended):       // El usuario deja de tocar la pantalla
                {
                    right = false;
                    left = false;


                    if((*event[ID(x)].as< var::Float > ()) > canvas_width * .9f
                       && (*event[ID(x)].as< var::Float > ()) < canvas_width * 1.f
                       && (*event[ID(y)].as< var::Float > ()) > canvas_height * .0f
                       && (*event[ID(y)].as< var::Float > ()) < canvas_height * .2f + 100){

                        move = false;

                    }


                    if((*event[ID(x)].as< var::Float > ()) > canvas_width * .0f
                    && (*event[ID(x)].as< var::Float > ()) < canvas_width * .1f + 100
                    && (*event[ID(y)].as< var::Float > ()) > canvas_height * .8f
                    && (*event[ID(y)].as< var::Float > ()) < canvas_height * 1.f){

                        state = PAUSE;

                    }

                    break;
                }
            }
        }
        else if(state == PAUSE){
            state = RUNNING;
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::update (float time)
    {
        if (!suspended) switch (state)
        {
            case LOADING: load_textures  ();     break;
            case RUNNING: run_simulation (time); break;
            case PAUSE: break;
            case ERROR:   break;
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::render (Context & context)
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

                switch (state)
                {
                    case LOADING: render_loading   (*canvas); break;
                    case RUNNING: render_playfield (*canvas); break;
                    case PAUSE: render_pause (*canvas); break;
                    case ERROR:   break;
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------
    // En este método solo se carga una textura por fotograma para poder pausar la carga si el
    // juego pasa a segundo plano inesperadamente. Otro aspecto interesante es que la carga no
    // comienza hasta que la escena se inicia para así tener la posibilidad de mostrar al usuario
    // que la carga está en curso en lugar de tener una pantalla en negro que no responde durante
    // un tiempo.

    void Game_Scene::load_textures ()
    {
        if (textures.size () < textures_count)          // Si quedan texturas por cargar...
        {
            // Las texturas se cargan y se suben al contexto gráfico, por lo que es necesario disponer
            // de uno:

            Graphics_Context::Accessor context = director.lock_graphics_context ();

            if (context)
            {
                // Se carga la siguiente textura (textures.size() indica cuántas llevamos cargadas):

                Texture_Data   & texture_data = textures_data[textures.size ()];
                Texture_Handle & texture      = textures[texture_data.id] = Texture_2D::create (texture_data.id, context, texture_data.path);

                // Se comprueba si la textura se ha podido cargar correctamente:

                if (texture) context->add (texture); else state = ERROR;
            }
        }
        else
        if (timer.get_elapsed_seconds () > 1.f)         // Si las texturas se han cargado muy rápido
        {                                               // se espera un segundo desde el inicio de
            create_sprites ();                          // la carga antes de pasar al juego para que
            restart_game   ();                          // el mensaje de carga no aparezca y desaparezca
                                                        // demasiado rápido.
            state = RUNNING;
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::create_sprites ()
    {
        Sprite_Handle    player_sprite(new Sprite( textures[ID(player)].get () ));

        player_sprite->set_anchor   (CENTER);
        player_sprite->set_position ({ canvas_width / 2, canvas_height / 2});
        player_sprite->set_size({ 50, 60});

        sprites.push_back ( player_sprite );

        // Se guardan punteros a los sprites que se van a usar frecuentemente:
        player   =  player_sprite.get ();
    }

    // ---------------------------------------------------------------------------------------------
    // Juando el juego se inicia por primera vez o cuando se reinicia porque un jugador pierde, se
    // llama a este método para restablecer la posición y velocidad de los sprites:

    void Game_Scene::restart_game()
    {
         player->set_position ({ canvas_width / 2.f, canvas_height / 2.f });
         player->set_speed  (Vector2f(0, 0));
         angle = 0;

        gameplay = WAITING_TO_START;
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::start_playing ()
    {
        gameplay = PLAYING;
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::run_simulation (float time)
    {
        if(gameplay == PLAYING){

            for (auto & sprite : sprites)
            {
                sprite->update (time);
            }
            for (auto & bullet : bullets)
            {
                bullet->update (time);

                if(bullet->get_position_x() > canvas_width + 10 &&
                   bullet->get_position_x() < -10 &&
                   bullet->get_position_y() > canvas_height + 10 &&
                   bullet->get_position_y() < -10){
                    bullet->hide();
                }
            }

            if(rand() % 10 == 0){
                Sprite_Handle newAsteroid(new Sprite( textures[ID(asteroide)].get () ));
                newAsteroid->set_anchor(CENTER);
                newAsteroid->set_size({ 100, 100});
                do {
                    newAsteroid->set_position ({ -150 + rand() % (( canvas_width + 150 + 1 ) + 150), -150 + rand() % (( canvas_height + 150 + 1 ) + 150)});
                }
                while (newAsteroid->get_position_x() < canvas_width + 100 &&
                       newAsteroid->get_position_x() > -100 &&
                       newAsteroid->get_position_y() < canvas_height + 100 &&
                       newAsteroid->get_position_y() > -100
                       );

                newAsteroid->set_speed(Vector2f(-100 + rand() % (( 100 + 1 ) - -100), -100 + rand() % (( 100 + 1 ) - -100)));

                asteroids.push_back(newAsteroid);
            }

            for (auto & asteroid : asteroids)
            {
                asteroid->update (time);

                if(asteroid->get_position_x() > canvas_width + 100 &&
                   asteroid->get_position_x() < -100 &&
                   asteroid->get_position_y() > canvas_height + 100 &&
                   asteroid->get_position_y() < -100){
                        asteroid->hide();
                }
            }
        }

        update_user ();
        check_collisions ();
    }

    // ---------------------------------------------------------------------------------------------
    // Se hace que el player dechero se mueva hacia arriba o hacia abajo según el usuario esté
    // tocando la pantalla por encima o por debajo de su centro. Cuando el usuario no toca la
    // pantalla se deja al player quieto.

    void Game_Scene::update_user ()
    {
        if(right){
            angle = angle - .1f;
        }
        if(left){
            angle = angle + .1f;
        }
        if(move){
            player->set_speed(Vector2f(cos(angle) * 0 - sin(angle) * 1, sin(angle) * 0 + cos(angle) * 1) * 200);
        }
        else{
            player->set_speed(Vector2f(0, 0));
        }

        if(player->get_position_x() < 25 || player->get_position_x() > canvas_width - 25){
            player->set_speed_x(0);
            if(player->get_position_x() < 25){
                player->set_position_x(25);
            }
            else{
                player->set_position_x(canvas_width - 25);
            }
        }
        if(player->get_position_y() < 30 || player->get_position_y() > canvas_height - 30){
            player->set_speed_y(0);
            if(player->get_position_y() < 30){
                player->set_position_y(30);
            }
            else{
                player->set_position_y(canvas_height - 30);
            }
        }

    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::check_collisions ()
    {
        if(gameplay == PLAYING){

            for (auto & asteroide : asteroids)
            {
                if(asteroide->is_visible() && asteroide->intersects(*player)){
                    ///Ir a escena GameOver
                }
            }
            for (auto & bullet : bullets)
            {
                for (auto & asteroide : asteroids)
                {
                    if(asteroide->is_visible() && bullet->is_visible() && asteroide->intersects(*bullet)){
                        asteroide->hide();
                        bullet->hide();

                        if(asteroide->get_size()[0] == 100){
                            for(int i = 0; i < 2; ++i){
                                Sprite_Handle newAsteroid(new Sprite( textures[ID(asteroide)].get () ));
                                newAsteroid->set_anchor(CENTER);
                                newAsteroid->set_size({ 50, 50});
                                newAsteroid->set_position (asteroide->get_position());

                                newAsteroid->set_speed(Vector2f(-100 + rand() % (( 100 + 1 ) - -100), -100 + rand() % (( 100 + 1 ) - -100)));

                                asteroids.push_back(newAsteroid);
                            }
                        }
                    }
                }
            }
        }
    }

    // ---------------------------------------------------------------------------------------------

    void Game_Scene::render_loading (Canvas & canvas)
    {
        Texture_2D * loading_texture = textures[ID(loading)].get ();

        if (loading_texture)
        {
            canvas.fill_rectangle
            (
                { canvas_width * .5f, canvas_height * .5f },
                { loading_texture->get_width (), loading_texture->get_height () },
                  loading_texture
            );
        }
    }

    // ---------------------------------------------------------------------------------------------
    // Simplemente se dibujan todos los sprites que conforman la escena.

    void Game_Scene::render_playfield (Canvas & canvas)
    {
        if(gameplay == PLAYING || gameplay == WAITING_TO_START){
            for (auto & sprite : sprites)
            {
                Transformation2f transformacion = rotate_then_translate_2d(angle, Vector2f(-(cos(angle) * player->get_position_x() - sin(angle) * player->get_position_y()) + player->get_position_x(), -(sin(angle) * player->get_position_x() + cos(angle) * player->get_position_y()) + player->get_position_y()));
                canvas.apply_transform(transformacion);
                sprite->render (canvas);
                canvas.set_transform (Transformation2f());
            }
            for (auto & bullet : bullets)
            {
                bullet->render (canvas);
            }
            for (auto & asteroid : asteroids)
            {
                asteroid->render (canvas);
            }

            canvas.fill_rectangle
                    (
                            { canvas_width * .9f, canvas_height * .2f },
                            { 100, 100 },
                            textures[ID(forward)]. get ()
                    );
        }
        if(gameplay == PLAYING){
            canvas.fill_rectangle
                    (
                            { canvas_width * .9f, canvas_height * .2f },
                            { 100, 100 },
                            textures[ID(forward)]. get ()
                    );
            canvas.fill_rectangle
                    (
                            { canvas_width * .8f, canvas_height * .2f },
                            { 100, 100 },
                            textures[ID(shoot)]. get ()
                    );

            canvas.fill_rectangle
                    (
                            { canvas_width * .1f, canvas_height * .2f },
                            { 100, 100 },
                            textures[ID(left)]. get ()
                    );
            canvas.fill_rectangle
                    (
                            { canvas_width * .2f, canvas_height * .2f },
                            { 100, 100 },
                            textures[ID(right)]. get ()
                    );

            canvas.fill_rectangle
                    (
                            { canvas_width * .1f, canvas_height * .85f },
                            { 30, 60 },
                            textures[ID(botonPausa)]. get ()
                    );
        }
    }

    void Game_Scene::render_pause (Canvas & canvas)
    {
        canvas.fill_rectangle
                (
                        { canvas_width * .5f, canvas_height * .5f },
                        { (textures[ID(pausa)]->get_width ()), (textures[ID(pausa)]->get_height ()) },
                        textures[ID(pausa)]. get ()
                );
    }

}
