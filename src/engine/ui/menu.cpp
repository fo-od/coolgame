#include "engine/ui/menu.hpp"

#include <SDL3_ttf/SDL_ttf.h>
#include <functional>
#include <iostream>
#include <ostream>
#include <ranges>
#include <string>

std::unordered_map< std::string, Menu > Menu::menus;

void Menu::create( const std::string& name, std::vector< std::unique_ptr< Element > > elements )
{
    menus[name] = Menu{};
    menus[name].mElements = std::move(elements);
}

void Menu::open( const std::string& name )
{
    try
    {
        menus.at(name).mVisible = true;
    }
    catch ( std::out_of_range& e )
    {
        std::cout << "Menu '" << name << "' does not exist: " << e.what() << std::endl;
    }
}

void Menu::close( const std::string& name )
{
    try
    {
        menus.at(name).mVisible = false;
    }
    catch ( std::out_of_range& e )
    {
        std::cout << "Menu '" << name << "' does not exist: " << e.what() << std::endl;
    }
}

void Menu::handle_input( const SDL_MouseMotionEvent& motion, const SDL_MouseButtonEvent& button )
{
    for ( const Menu& menu : menus | std::views::values )
    {
        if ( menu.mVisible )
        {
            for ( const auto& element : menu.mElements )
            {
                element->handle_input(motion, button);
            }
        }
    }
}

void Menu::update( const float canvasWidth, const float canvasHeight )
{
    for ( const Menu& menu : menus | std::views::values )
    {
        if ( menu.mVisible )
        {
            for ( const auto& element : menu.mElements )
            {
                element->update_position(canvasWidth, canvasHeight);
            }
        }
    }
}

void Menu::draw( SDL_Renderer *renderer )
{
    for ( const Menu& menu : menus | std::views::values )
    {
        if ( menu.mVisible )
        {
            for ( const auto& element : menu.mElements )
            {
                element->draw(renderer);
            }
        }
    }
}