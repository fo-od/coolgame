#include "menu.hpp"

#include <functional>
#include <ranges>
#include <string>

#include "util.hpp"
#include "SDL3_ttf/SDL_ttf.h"

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
    menus.at(name).mVisible = true;
}

void Menu::close( const std::string &name )
{
    menus.at(name).mVisible = false;
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