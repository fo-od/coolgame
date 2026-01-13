#include "engine/level/creator.hpp"

#include <SDL3/SDL_asyncio.h>
#include <SDL3/SDL_dialog.h>
#include <SDL3/SDL_log.h>

SDL_DialogFileFilter filter[] = {"Level file", "cgl"};
SDL_AsyncIOQueue *ioQueue = SDL_CreateAsyncIOQueue();
SDL_AsyncIO *file;

void SDLCALL levelFileCallback( void *userdata, const char *const*filepath, const int numFilter )
{
    if ( !filepath )
    {
        SDL_Log("An error occurred: %s", SDL_GetError());
        return;
    }
    if ( !*filepath )
    {
        SDL_Log("The user did not select any file.");
        return;
    }

    while ( *filepath )
    {
        SDL_Log("Full path to selected file: '%s'", *filepath);
        filepath++;
    }

    if ( numFilter < 0 )
    {
        SDL_Log("The current platform does not support fetching "
                "the selected filter, or the user did not select"
                " any filter.");
    } else if ( numFilter < SDL_arraysize(filter) )
    {
        SDL_Log("The filter selected by the user is '%s' (%s).",
                filter[numFilter].pattern, filter[numFilter].name);
    }

    file = SDL_AsyncIOFromFile(*filepath, "rw");
}

void openLevel( SDL_Window *window )
{
    SDL_ShowOpenFileDialog(levelFileCallback, nullptr, window, filter, 1, nullptr, false);
    // todo: load level into game
}

void writeToLevel( void *ptr, const Uint64 offset, const Uint64 size )
{
    if ( !file )
        return;
    SDL_WriteAsyncIO(file, ptr, offset, size, ioQueue, nullptr);
}

void readFromLevel( void *ptr, const Uint64 offset, const Uint64 size )
{
    if ( !file )
        return;
    SDL_ReadAsyncIO(file, ptr, offset, size, ioQueue, nullptr);
}
