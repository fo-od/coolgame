#include <filesystem>

#include "engine/physics/physics.hpp"
#include "engine/game_state.hpp"
#include "engine/ui/button.hpp"
#include "engine/ui/menu.hpp"
#include "util/types.hpp"
#include "util/util.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

static SDL_Window *mWindow{};
static SDL_Renderer *mRenderer{};
static TTF_Font *mFont;
static TTF_TextEngine *mTextEngine;

static const bool *keyboardState{};
static std::string mCurrentMenu;

static float deltaTime = 0.0f;
static float camX = 0.0f;
static float camY = 0.0f;

static bool mRunning = false;
static GameState mGameState = GameState::InMenu;

static int mWidth = 640;
static int mHeight = 480;

static void init()
{
    // sdl setup stuff
    SDL_SetAppMetadata("cool game", "0.1", "com.food.coolgame");
    SDL_Init(SDL_INIT_VIDEO);

    mWindow = SDL_CreateWindow("cool game", mWidth, mHeight, SDL_WINDOW_RESIZABLE |
                                                             SDL_WINDOW_BORDERLESS);
    mRenderer = SDL_CreateRenderer(mWindow, nullptr);

    // enable vsync
    SDL_SetRenderVSync(mRenderer, 1);

    // blend alpha channel
    SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);

    TTF_Init();
    mTextEngine = TTF_CreateRendererTextEngine(mRenderer);

    const std::filesystem::path basePath = SDL_GetBasePath();
    const auto fontPath = basePath / "assets/cozette.fnt";
    mFont = TTF_OpenFont(fontPath.string().c_str(), 13);

    mRunning = true;

    OPEN_MENU("mainMenu")
}

static void exit()
{
    TTF_CloseFont(mFont);
    TTF_Quit();

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

static void input( SDL_Event *e )
{
    if ( e->type == SDL_EVENT_QUIT )
    {
        mRunning = false;
    }

    if ( e->type == SDL_EVENT_WINDOW_RESIZED )
    {
        SDL_GetWindowSizeInPixels(mWindow, &mWidth, &mHeight);
        Menu::update(mWidth, mHeight);
    }

    keyboardState = SDL_GetKeyboardState(nullptr);

    if ( e->type == SDL_EVENT_KEY_DOWN )
    {
        if ( e->key.key == SDLK_ESCAPE )
        {
            if ( !(mGameState & GameState::InMenu) )
            {
                OPEN_MENU("pauseMenu")
            } else
            {
                CLOSE_MENU(mCurrentMenu)
            }
        }
    }

    if ( e->type == SDL_EVENT_MOUSE_MOTION ||
         e->type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
         e->type == SDL_EVENT_MOUSE_BUTTON_UP )
    {
        SDL_ConvertEventToRenderCoordinates(mRenderer, e);

        if ( mGameState & GameState::InMenu )
        {
            Menu::handle_input(e->motion, e->button);
        }
    }
}

static void update( Body *player )
{
    Vector2 velocity{0.0, player->mVelocity.y};

    if ( keyboardState[SDL_SCANCODE_LEFT] )
    {
        velocity.x -= 1000;
    }
    if ( keyboardState[SDL_SCANCODE_RIGHT] )
    {
        velocity.x += 1000;
    }
    if ( keyboardState[SDL_SCANCODE_UP] )
    {
        velocity.y = -2000;
    }
    if ( keyboardState[SDL_SCANCODE_DOWN] )
    {
        velocity.y += 800;
    }

    player->mVelocity = velocity;

    Physics::update(deltaTime);
}

static void render()
{
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(mRenderer);

    if ( mGameState & GameState::InMenu )
    {
        // draw the world in a darker color for better menu visibility
        SDL_SetRenderDrawColor(mRenderer, 127, 127, 127, SDL_ALPHA_OPAQUE);
        AABB::draw(mRenderer);

        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        Menu::draw(mRenderer);
    } else
    {
        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        AABB::draw(mRenderer);
    }

    SDL_RenderPresent(mRenderer);
}

static void createMenu() {}

static void create_menus()
{
    Menu::create("pauseMenu", Element::make_vector(
                                                   std::make_unique< Button >(mTextEngine, mFont,
                                                                              0, 20,
                                                                              Anchor::Center, Anchor::Center,
                                                                              "Resume",
                                                                              []
                                                                              {
                                                                                  CLOSE_MENU(mCurrentMenu)
                                                                              }),
                                                   std::make_unique< Button >(mTextEngine, mFont,
                                                                              0, -20,
                                                                              Anchor::Center, Anchor::Center,
                                                                              "Exit",
                                                                              []
                                                                              {
                                                                                  CLOSE_MENU(mCurrentMenu)
                                                                                  mRunning = false;
                                                                              })
                                                  ));

    Menu::create("mainMenu", Element::make_vector(
                                                  std::make_unique< Button >(mTextEngine, mFont, 0, 20,
                                                                             Anchor::Center, Anchor::Center,
                                                                             "Levels",
                                                                             []
                                                                             {
                                                                                 CLOSE_MENU(mCurrentMenu)
                                                                                 OPEN_MENU("pauseMenu")
                                                                                 // todo: make a level select menu
                                                                                 mGameState =
                                                                                     mGameState | GameState::Playing;
                                                                             }),
                                                  std::make_unique< Button >(mTextEngine, mFont, 0, -20,
                                                                             Anchor::Center, Anchor::Center,
                                                                             "Create",
                                                                             []
                                                                             {
                                                                                 mGameState =
                                                                                     mGameState | GameState::Editing;
                                                                                 createMenu();
                                                                                 CLOSE_MENU(mCurrentMenu)
                                                                             })
                                                 ));
}

int main()
{
    init();
    create_menus();

    const u32 player = Physics::add_body(Vector2{mWidth / 2, mHeight / 2}, Vector2{10, 10}, true);

    const u32 floor = Physics::add_static_body(Vector2{mWidth / 2, mHeight - 25},
                                               Vector2{mWidth / 2, 25});
    const u32 left_wall = Physics::add_static_body(Vector2{25, mHeight / 2},
                                                   Vector2{25, mHeight / 2});
    const u32 right_wall = Physics::add_static_body(Vector2{mWidth - 25, mHeight / 2},
                                                    Vector2{25, mHeight / 2});
    const u32 ceiling = Physics::add_static_body(Vector2{mWidth / 2, 25}, Vector2{mWidth / 2, 25});

    while ( mRunning )
    {
        // todo: let player add and edit platforms (in game loop)
        const u64 currentTick = SDL_GetTicks();

        SDL_Event event;
        while ( SDL_PollEvent(&event) )
        {
            input(&event);
        }

        if ( !(mGameState & GameState::InMenu) )
        {
            update(Physics::get_body(player));
        }

        render();

        deltaTime = (SDL_GetTicks() - currentTick) / 1000.0f;
    }

    exit();

    return 0;
}
