#pragma once
#include <functional>

#include "element.hpp"
#include <SDL3_ttf/SDL_ttf.h>

class Menu
{
    static std::unordered_map<std::string, Menu> menus;

    std::vector<std::shared_ptr<Element> > mElements;
    bool mVisible = false;

    public:
        static void create( const std::string &name, const std::vector<std::shared_ptr<Element> > &elements );

        static void open( const std::string &name );

        static void close( const std::string &name );

        static void update( const SDL_MouseMotionEvent &motion, const SDL_MouseButtonEvent &button, int width,
                            int height );

        static void draw( SDL_Renderer *renderer );
};