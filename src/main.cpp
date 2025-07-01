#include "main.hpp"

#include "button.hpp"
#include "constants.hpp"
#include "globals.hpp"
#include "io.hpp"
#include "physics.hpp"
#include "types.hpp"
#include "util.hpp"

#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

int main()
{
    if ( !init_sdl() ) {
        return 1;
    }

    const Button startButton("Start", ANCHOR_MIDDLE_MIDDLE, RENDER_WIDTH / 2, RENDER_HEIGHT / 2, []() {
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

    const u32 player_id = Physics::add_body(Vector2{RENDER_WIDTH / 2, RENDER_HEIGHT / 2}, Vector2{10, 10}, true);

    const u32 floor = Physics::add_static_body(Vector2{RENDER_WIDTH / 2, RENDER_HEIGHT - 25},
                                               Vector2{RENDER_WIDTH / 2, 25});
    const u32 left_wall = Physics::add_static_body(Vector2{25, RENDER_HEIGHT / 2},
                                                   Vector2{25, RENDER_HEIGHT / 2});
    const u32 right_wall = Physics::add_static_body(Vector2{RENDER_WIDTH - 25, RENDER_HEIGHT / 2},
                                                    Vector2{25, RENDER_HEIGHT / 2});
    const u32 ceiling = Physics::add_static_body(Vector2{RENDER_WIDTH / 2, 25}, Vector2{RENDER_WIDTH / 2, 25});

    u32 lastTicks = SDL_GetTicks();

    while ( gameIsRunning ) {
        // calculate delta time
        const u32 nowTicks = SDL_GetTicks();
        deltaTime = ( nowTicks - lastTicks ) * 0.001f;
        lastTicks = nowTicks;

        SDL_Event event;
        while ( SDL_PollEvent(&event) ) {
            handle_events(&event);
        }
        update(Physics::get_body(player_id));
        render();
    }

    cleanup();
    return 0;
}

bool init_sdl()
{
    // sdl stuff
    HANDLE_SDL_ERROR_RETURN(SDL_SetAppMetadata("cool game", "1.0", "com.food.coolgame"),
                            "Couldn't set app metadata: %s")

    HANDLE_SDL_ERROR_RETURN(SDL_Init(SDL_INIT_VIDEO), "Couldn't initialize SDL: %s")

    HANDLE_SDL_ERROR_RETURN(SDL_CreateWindowAndRenderer("cool game", 1024, 768, SDL_WINDOW_RESIZABLE |
                                SDL_WINDOW_MAXIMIZED,
                                &window,
                                &renderer), "Couldn't create window/renderer: %s")
    // enable vsync
    HANDLE_SDL_ERROR_RETURN(SDL_SetRenderVSync(renderer, 1), "Couldn't enable VSync: %s")

    // do things in 640 x 480 but scale it as needed
    HANDLE_SDL_ERROR_RETURN(SDL_SetRenderLogicalPresentation(renderer, 640, 480,
                                SDL_LOGICAL_PRESENTATION_LETTERBOX),
                            "Couldn't set the logical presentation: %s")

    // use the alpha channel for transparency
    HANDLE_SDL_ERROR_RETURN(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND), "Couldn't set blend mode: %s")

    // setup ttf things
    HANDLE_SDL_ERROR_RETURN(TTF_Init(), "Couldn't initialize text renderer: %s")

    HANDLE_SDL_ERROR_RETURN(textEngine = TTF_CreateRendererTextEngine(renderer), "Couldn't create text engine: %s")

    HANDLE_SDL_ERROR_RETURN(font = TTF_OpenFontIO(SDL_IOFromConstMem(cozette, cozette_len), true, FONT_SIZE),
                            "Couldn't load font: %s")

    return true;
}

void handle_events( SDL_Event *e )
{
    if ( e->type == SDL_EVENT_QUIT )
        gameIsRunning = false;


    update_mouse(e);
}

void update( Body *player )
{
    update_keyboard();

    Vector2 velocity{0.0, player->velocity.y};

    if ( keyboardState[SDL_SCANCODE_LEFT] ) {
        velocity.x -= 1000;
    }
    if ( keyboardState[SDL_SCANCODE_RIGHT] ) {
        velocity.x += 1000;
    }
    if ( keyboardState[SDL_SCANCODE_UP] ) {
        velocity.y = -2000;
    }
    if ( keyboardState[SDL_SCANCODE_DOWN] ) {
        velocity.y += 800;
    }

    player->velocity = velocity;

    Physics::update();
}

void render()
{
    U_SetRenderDrawColor(COLOR_BLACK);
    HANDLE_SDL_ERROR(SDL_RenderClear(renderer), "Couldn't clear screen: %s")

    U_SetRenderDrawColor(COLOR_WHITE);
    AABB::draw();

    HANDLE_SDL_ERROR(SDL_RenderPresent(renderer), "Couldn't update screen: %s")
}

void cleanup()
{
    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
