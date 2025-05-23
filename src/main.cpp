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

Body *a;
Body *b;

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
    HANDLE_SDL_ERROR_RETURN(SDL_SetAppMetadata("cool game", "1.0", "com.food.coolgame"),
                            "Couldn't set app metadata: {}");

    HANDLE_SDL_ERROR_RETURN(SDL_Init(SDL_INIT_VIDEO), "Couldn't initialize SDL: {}");

    HANDLE_SDL_ERROR_RETURN(SDL_CreateWindowAndRenderer("cool game", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL, &
                                window,
                                &renderer), "Couldn't create window/renderer: {}");

    // enable vsync
    HANDLE_SDL_ERROR_RETURN(SDL_SetRenderVSync(renderer, 1), "Couldn't enable VSync: {}");

    // setup ttf things
    HANDLE_SDL_ERROR_RETURN(TTF_Init(), "Couldn't initialize text renderer: {}");

    textEngine = TTF_CreateRendererTextEngine(renderer);
    HANDLE_SDL_ERROR_RETURN(textEngine, "Couldn't create text engine: {}");

    font = TTF_OpenFontIO(SDL_IOFromConstMem(cozette, cozette_len), true, FONT_SIZE);
    HANDLE_SDL_ERROR_RETURN(font, "Couldn't load font: {}");

    return true;
}

bool init_game()
{
    a = Physics::add_body(Vector2{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, Vector2{50, 50});
    b = Physics::add_body(Vector2{0, 0}, Vector2{100, 100});
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

    b->aabb.pos = mouse.pos;
    if (b->aabb.intersects(a->aabb)) {
        SDL_Log("hi");
    }
}

void render()
{
    U_SetRenderDrawColor(COLOR_BLACK);
    HANDLE_SDL_ERROR(SDL_RenderClear(renderer), "Could not clear screen: {}");

    // draw stuff here
    U_SetRenderDrawColor(COLOR_WHITE);
    AABB::draw();

    HANDLE_SDL_ERROR(SDL_RenderPresent(renderer), "Could not update the screen: {}");
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
