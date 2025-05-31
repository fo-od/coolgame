#include "main.hpp"

#include "button.hpp"
#include "constants.hpp"
#include "globals.hpp"
#include "io.hpp"
#include "physics.hpp"
#include "player.hpp"
#include "ray.hpp"
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
    HANDLE_SDL_ERROR_RETURN(SDL_SetAppMetadata("cool game", "1.0", "com.food.coolgame"),
                            "Couldn't set app metadata: %s");

    HANDLE_SDL_ERROR_RETURN(SDL_Init(SDL_INIT_VIDEO), "Couldn't initialize SDL: %s");

    HANDLE_SDL_ERROR_RETURN(SDL_CreateWindowAndRenderer("cool game", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL, &
                                window,
                                &renderer), "Couldn't create window/renderer: %s");

    // enable vsync
    HANDLE_SDL_ERROR_RETURN(SDL_SetRenderVSync(renderer, 1), "Couldn't enable VSync: %s");

    // use alpha channel for transparency
    HANDLE_SDL_ERROR_RETURN(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND), "Couldn't set blend mode: %s");

    // setup ttf things
    HANDLE_SDL_ERROR_RETURN(TTF_Init(), "Couldn't initialize text renderer: %s");

    HANDLE_SDL_ERROR_RETURN(textEngine = TTF_CreateRendererTextEngine(renderer), "Couldn't create text engine: %s");

    HANDLE_SDL_ERROR_RETURN(font = TTF_OpenFontIO(SDL_IOFromConstMem(cozette, cozette_len), true, FONT_SIZE),
                            "Couldn't load font: %s");

    return true;
}

Body test_body;
Body mouse_body;
Body start_body;

bool init_game()
{
    Physics::add_body(&( test_body = Body{Vector2{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, Vector2{50, 50}} ));
    Physics::add_body(&( mouse_body = Body{Vector2{0, 0}, Vector2{75, 75}} ));
    Physics::add_body(&( start_body = Body{mouse_body} ));

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

    if ( mouse.buttons & SDL_BUTTON_LEFT ) {
        start_body.aabb.pos = mouse.pos;
    }
    mouse_body.aabb.pos = mouse.pos;
}

void render()
{
    U_SetRenderDrawColor(31, 31, 30);
    HANDLE_SDL_ERROR(SDL_RenderClear(renderer), "Could not clear screen: %s");

    const AABB sum_aabb{test_body.aabb.pos, test_body.aabb.half_size + mouse_body.aabb.half_size};

    U_RenderRect(&test_body.aabb.rect, 255, 255, 255);
    U_RenderRect(&start_body.aabb.rect, 255, 255, 255);
    U_RenderRect(&mouse_body.aabb.rect, 255, 255, 255, 175);
    U_RenderRect(&sum_aabb.rect, 255, 255, 255, 175);

    U_RenderLine(start_body.aabb.pos, mouse_body.aabb.pos);

    if ( const Hit hit{
        sum_aabb.intersects(start_body.aabb.pos, mouse_body.aabb.pos - start_body.aabb.pos)
    }; hit.is_hit ) {
        const AABB res{hit.position, mouse_body.aabb.half_size};
        U_RenderRect(&res.rect, 0, 255, 255);
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
