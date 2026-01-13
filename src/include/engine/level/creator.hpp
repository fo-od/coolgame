#pragma once
#include <SDL3/SDL_asyncio.h>
#include <SDL3/SDL_video.h>

void SDLCALL levelFileCallback( void *userdata, const char *const*filepath, int numFilter );

void openLevel( SDL_Window *window );

void writeToLevel( void *ptr, Uint64 offset, Uint64 size );

void readFromLevel( void *ptr, Uint64 offset, Uint64 size );
