#include "menu.hpp"

#include <functional>
#include <iostream>
#include <ostream>
#include <ranges>
#include <string>

#include "util.hpp"
#include <SDL3_ttf/SDL_ttf.h>

std::unordered_map<std::string, Menu> Menu::menus = {};

void Menu::create( const std::string &name, const std::vector<std::shared_ptr<Element> > &elements )
{
    menus[name] = Menu{};
    for (const auto &element: elements) {
        menus[name].mElements.emplace_back(element);
    }
}

void Menu::open( const std::string &name )
{
    try {
        menus.at(name).mVisible = true;
    } catch (std::out_of_range &e) {
        std::cout << "Menu '" << name << "' does not exist: " << e.what() << std::endl;
    }
}

void Menu::close( const std::string &name )
{
    try {
        menus.at(name).mVisible = false;
    } catch (std::out_of_range &e) {
        std::cout << "Menu '" << name << "' does not exist: " << e.what() << std::endl;
    }
}

void Menu::update( const SDL_MouseMotionEvent &motion, const SDL_MouseButtonEvent &button )
{
    for (const Menu &menu: menus | std::views::values) {
        if (menu.mVisible) {
            for (auto &element: menu.mElements) {
                element->update(motion, button);
            }
        }
    }
}

void Menu::draw( SDL_Renderer *renderer )
{
    for (const Menu &menu: menus | std::views::values) {
        if (menu.mVisible) {
            for (const auto &element: menu.mElements) {
                element->draw(renderer);
            }
        }
    }
}