#include "main.hpp"

#include "button.hpp"
#include "constants.hpp"
#include "globals.hpp"
#include "io.hpp"
#include "physics.hpp"
#include "player.hpp"
#include "types.hpp"
#include "util.hpp"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

int main()
{
    if ( !init_sdl() ) {
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

    init_game();
    SDL_Log("Game started");

    u32 lastTicks = SDL_GetTicks();

    while ( gameIsRunning ) {
        const u32 nowTicks = SDL_GetTicks();
        deltaTime = ( nowTicks - lastTicks ) * 0.001f;
        lastTicks = nowTicks;

        SDL_Event event;
        while ( SDL_PollEvent(&event) ) {
            handle_events(&event);
        }
        update();
        render();
    }

    cleanup();
    return 0;
}

bool init_sdl()
{
    // sdl stuff
    HANDLE_SDL_ERROR(SDL_SetAppMetadata("cool game", "1.0", "com.food.coolgame"), "Couldn't set app metadata: %s");

    HANDLE_SDL_ERROR(SDL_Init(SDL_INIT_VIDEO), "Couldn't initialize SDL: %s");

    HANDLE_SDL_ERROR(SDL_CreateWindowAndRenderer("cool game", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL, &window,
                         &renderer), "Couldn't create window/renderer: %s");

    // enable vsync
    HANDLE_SDL_ERROR(SDL_SetRenderVSync(renderer, 1), "Couldn't enable VSync: %s");

    // setup ttf things
    HANDLE_SDL_ERROR(TTF_Init(), "Couldn't initialize text renderer: %s");

    textEngine = TTF_CreateRendererTextEngine(renderer);
    HANDLE_SDL_ERROR(textEngine, "Couldn't create text engine: %s");

    font = TTF_OpenFontIO(SDL_IOFromConstMem(mago_ttf, mago_ttf_len), true, FONT_SIZE);
    HANDLE_SDL_ERROR(font, "Couldn't load font: %s");

    return true;
}

bool init_game()
{
    Physics::add_body(Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), Vector2(100, 100));
    return true;
}

void handle_events( const SDL_Event *e )
{
    if ( e->type == SDL_EVENT_QUIT )
        gameIsRunning = false;


    if ( e->type == SDL_EVENT_MOUSE_MOTION || e->type == SDL_EVENT_MOUSE_BUTTON_DOWN || e->type ==
         SDL_EVENT_MOUSE_BUTTON_UP )
        update_mouse(e);
}


void update()
{
    update_keyboard();
    Physics::update();
}

void render()
{
    U_SetRenderDrawColor(COLOR_BLACK);
    SDL_RenderClear(renderer);

    // draw stuff here
    Physics::draw();

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
