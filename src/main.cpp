#include <filesystem>

#include "engine/ui/button.hpp"
#include "engine/physics.hpp"
#include "engine/ui/menu.hpp"
#include "util/types.hpp"
#include "util/util.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

struct SDLApplication
{
    SDL_Window *mWindow{};
    SDL_Renderer *mRenderer{};
    TTF_Font *mFont;
    TTF_TextEngine *mTextEngine;

    const bool *keyboardState{};
    std::string mCurrentMenu;

    float deltaTime = 0.0f;
    float camX = 0.0f;
    float camY = 0.0f;

    bool mRunning = false;
    bool mInMenu = false;

    int mWidth = 640;
    int mHeight = 480;

    SDLApplication()
    {
        // sdl setup stuff
        HANDLE_SDL_ERROR(SDL_SetAppMetadata("cool game", "0.1", "com.food.coolgame"),
                         "Couldn't set app metadata: %s")

        HANDLE_SDL_ERROR(SDL_Init(SDL_INIT_VIDEO), "Couldn't initialize SDL: %s")

        HANDLE_SDL_ERROR(
            (mWindow = SDL_CreateWindow("cool game", mWidth, mHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS)),
            "Couldn't create window: %s")
        HANDLE_SDL_ERROR((mRenderer = SDL_CreateRenderer(mWindow, nullptr)), "Couldn't create renderer: %s")

        // enable vsync
        HANDLE_SDL_ERROR(SDL_SetRenderVSync(mRenderer, 1), "Couldn't enable VSync: %s")

        // blend alpha channel
        HANDLE_SDL_ERROR(SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND), "Couldn't set blend mode: %s")

        // setup ttf things
        HANDLE_SDL_ERROR(TTF_Init(), "Couldn't initialize text renderer: %s")

        HANDLE_SDL_ERROR((mTextEngine = TTF_CreateRendererTextEngine(mRenderer)), "Couldn't create text engine: %s")

        const std::filesystem::path basePath = SDL_GetBasePath();
        const auto fontPath = basePath / "assets/cozette.fnt";
        HANDLE_SDL_ERROR((mFont = TTF_OpenFont(fontPath.string().c_str(), 13)),
                         "Couldn't load font: %s")

        //menus
        Menu::create("pauseMenu", Element::make_vector(
                         std::make_unique<Button>(mTextEngine, mFont,
                                                  0, 20,
                                                  Anchor::Center, Anchor::Center,
                                                  "Resume",
                                                  [this] {
                                                      CLOSE_MENU(mCurrentMenu)
                                                  }),
                         std::make_unique<Button>(mTextEngine, mFont,
                                                  0, -20,
                                                  Anchor::Center, Anchor::Center,
                                                  "Exit",
                                                  [this] {
                                                      CLOSE_MENU(mCurrentMenu)
                                                      mRunning = false;
                                                  })
                     ));

        Menu::create("mainMenu", Element::make_vector(
                         std::make_unique<Button>(mTextEngine, mFont, 0, 20, Anchor::Center, Anchor::Center,
                                                  "Levels",
                                                  [this] {
                                                      CLOSE_MENU(mCurrentMenu)
                                                  }),
                         std::make_unique<Button>(mTextEngine, mFont, 0, -20, Anchor::Center, Anchor::Center,
                                                  "Create",
                                                  [this] {
                                                      CLOSE_MENU(mCurrentMenu)
                                                  })
                     ));
    }

    ~SDLApplication()
    {
        TTF_CloseFont(mFont);
        TTF_Quit();

        SDL_DestroyRenderer(mRenderer);
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
    }

    void mainLoop()
    {
        mRunning = true;

        OPEN_MENU("mainMenu")

        const u32 player = Physics::add_body(Vector2{mWidth / 2, mHeight / 2}, Vector2{10, 10}, true);

        const u32 floor = Physics::add_static_body(Vector2{mWidth / 2, mHeight - 25},
                                                   Vector2{mWidth / 2, 25});
        const u32 left_wall = Physics::add_static_body(Vector2{25, mHeight / 2},
                                                       Vector2{25, mHeight / 2});
        const u32 right_wall = Physics::add_static_body(Vector2{mWidth - 25, mHeight / 2},
                                                        Vector2{25, mHeight / 2});
        const u32 ceiling = Physics::add_static_body(Vector2{mWidth / 2, 25}, Vector2{mWidth / 2, 25});

        while (mRunning) {
            const u64 currentTick = SDL_GetTicks();

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                input(&event);
            }

            if (!mInMenu) {
                update(Physics::get_body(player));
            }

            render();

            deltaTime = static_cast<float>(SDL_GetTicks() - currentTick) / 1000.0f;
        }
    }

    void input( SDL_Event *e )
    {
        if (e->type == SDL_EVENT_QUIT) {
            mRunning = false;
        }

        if (e->type == SDL_EVENT_WINDOW_RESIZED) {
            SDL_GetWindowSizeInPixels(mWindow, &mWidth, &mHeight);
            Menu::update(mWidth, mHeight);
        }

        keyboardState = SDL_GetKeyboardState(nullptr);

        if (e->type == SDL_EVENT_KEY_DOWN) {
            if (e->key.key == SDLK_ESCAPE) {
                if (!mInMenu) {
                    OPEN_MENU("pauseMenu")
                } else {
                    CLOSE_MENU(mCurrentMenu)
                }
            }
        }

        if (e->type == SDL_EVENT_MOUSE_MOTION ||
            e->type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
            e->type == SDL_EVENT_MOUSE_BUTTON_UP) {
            SDL_ConvertEventToRenderCoordinates(mRenderer, e);

            if (mInMenu) {
                Menu::handle_input(e->motion, e->button);
            }
        }
    }

    void update( Body *player ) const
    {
        Vector2 velocity{0.0, player->mVelocity.y};

        if (keyboardState[SDL_SCANCODE_LEFT]) {
            velocity.x -= 1000;
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            velocity.x += 1000;
        }
        if (keyboardState[SDL_SCANCODE_UP]) {
            velocity.y = -2000;
        }
        if (keyboardState[SDL_SCANCODE_DOWN]) {
            velocity.y += 800;
        }

        player->mVelocity = velocity;

        Physics::update(deltaTime);
    }

    void render() const
    {
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(mRenderer);

        if (mInMenu) {
            // draw the world in a darker color for better menu visibility
            SDL_SetRenderDrawColor(mRenderer, 127, 127, 127, SDL_ALPHA_OPAQUE);
            AABB::draw(mRenderer);

            SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            Menu::draw(mRenderer);
        } else {
            SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            AABB::draw(mRenderer);
        }

        SDL_RenderPresent(mRenderer);
    }
};

int main()
{
    SDLApplication app;
    app.mainLoop();
    return 0;
}
