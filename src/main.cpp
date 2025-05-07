#include "main.hpp"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "button.hpp"
#include "constants.hpp"
#include "globals.hpp"
#include "player.hpp"
#include "util.hpp"

int main()
{
    if ( !init() ) {
        return 1;
    }

    const Button startButton("Start", ANCHOR_MIDDLE_MIDDLE, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, []() {
        inMainMenu = false;
        gameIsRunning = true;
    });

    while ( inMainMenu ) {
        SDL_Event event;
        while ( SDL_PollEvent(&event) ) {
            if ( event.type == SDL_EVENT_QUIT ) {
                inMainMenu = false;
            }
            update_mouse(&event);
        }
        startButton.update();
        SDL_RenderPresent(renderer);
    }

    SDL_Log("Game started");

    Player player(0, 0, 20, 20, 0.85);

    Uint32 lastTicks = SDL_GetTicks();

    while ( gameIsRunning ) {
        const Uint32 nowTicks = SDL_GetTicks();
        // Dt in seconds
        deltaTime = ( nowTicks - lastTicks ) * 0.001f;
        lastTicks = nowTicks;

        SDL_Event event;
        while ( SDL_PollEvent(&event) ) {
            handle_events(&event);
        }
        update(&player);
        render(&player);
    }

    cleanup();
    return 0;
}

bool init()
{
    // sdl stuff
    if ( !SDL_SetAppMetadata("cool game", "1.0", "com.food.coolgame") ) {
        SDL_Log("Couldn't set app metadata: %s", SDL_GetError());
        return false;
    }

    if ( !SDL_Init(SDL_INIT_VIDEO) ) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }

    if ( !SDL_CreateWindowAndRenderer("cool game", 640, 480, SDL_WINDOW_OPENGL, &window,
                                      &renderer) ) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return false;
    }

    // enable vsync
    if ( !SDL_SetRenderVSync(renderer, 1) ) {
        SDL_Log("Couldn't enable VSync: %s", SDL_GetError());
        return false;
    }

    // setup ttf things
    if ( !TTF_Init() ) {
        SDL_Log("Couldn't initialize text renderer: %s", SDL_GetError());
        return false;
    }

    textEngine = TTF_CreateRendererTextEngine(renderer);
    if ( !textEngine ) {
        SDL_Log("Couldn't create text engine: %s", SDL_GetError());
        return false;
    }

    font = TTF_OpenFontIO(SDL_IOFromConstMem(mago_ttf, mago_ttf_len), true, FONT_SIZE);
    if ( !font ) {
        SDL_Log("Couldn't load font: %s", SDL_GetError());
        return false;
    }

    return true;
}

void handle_events( const SDL_Event *e )
{
    if ( e->type == SDL_EVENT_QUIT ) {
        gameIsRunning = false;
    }

    update_mouse(e);
}


void update( Player *player )
{
    update_keyboard();

    player->update();
}

void render( const Player *player )
{
    U_SetRenderDrawColor(COLOR_BLACK);
    SDL_RenderClear(renderer);

    // render stuff here
    player->draw();

    SDL_RenderPresent(renderer);
}

void cleanup()
{
    // sdl cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    // ttf cleanup
    TTF_CloseFont(font);
    TTF_Quit();
}
