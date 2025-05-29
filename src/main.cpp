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

Body a;
Body b;

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
                            "Couldn't set app metadata: %s");

    HANDLE_SDL_ERROR_RETURN(SDL_Init(SDL_INIT_VIDEO), "Couldn't initialize SDL: %s");

    HANDLE_SDL_ERROR_RETURN(SDL_CreateWindowAndRenderer("cool game", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL, &
                                window,
                                &renderer), "Couldn't create window/renderer: %s");

    // enable vsync
    HANDLE_SDL_ERROR_RETURN(SDL_SetRenderVSync(renderer, 1), "Couldn't enable VSync: %s");

    // setup ttf things
    HANDLE_SDL_ERROR_RETURN(TTF_Init(), "Couldn't initialize text renderer: %s");

    HANDLE_SDL_ERROR_RETURN(textEngine = TTF_CreateRendererTextEngine(renderer), "Couldn't create text engine: %s");

    HANDLE_SDL_ERROR_RETURN(font = TTF_OpenFontIO(SDL_IOFromConstMem(cozette, cozette_len), true, FONT_SIZE),
                            "Couldn't load font: %s");

    return true;
}

bool init_game()
{
    Physics::add_body(&( a = Body{Vector2{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, Vector2{100, 100}} ));
    Physics::add_body(&( b = Body{Vector2{0, 0}, Vector2{50, 50}} ));

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

    b.aabb.pos = mouse.pos;
}

void render()
{
    U_SetRenderDrawColor(COLOR_BLACK);
    HANDLE_SDL_ERROR(SDL_RenderClear(renderer), "Could not clear screen: %s");

    // draw stuff here
    U_SetRenderDrawColor(COLOR_WHITE);
    AABB::draw();
    if ( b.aabb.intersects(a.aabb) ) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderRect(renderer, &b.aabb.rect);
        const Vector2 pen{b.aabb.penetration_vector(a.aabb)};
        pen.draw(b.aabb.pos);
        const AABB res{pen+b.aabb.pos, b.aabb.half_size};
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_RenderRect(renderer, &res.rect);
    }

    HANDLE_SDL_ERROR(SDL_RenderPresent(renderer), "Could not update the screen: %s");
}

void cleanup()
{
    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
