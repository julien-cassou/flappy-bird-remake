#ifndef MENU_HPP
#define MENU_HPP
#include "jeu.hpp"


class Menu {
    private:
        Jeu jeu;
    public: 
        Menu() : jeu{BirdType::Sonic} {};
        void MainMenu(sf::RenderWindow &window);
};

sf::Sprite setTitle(sf::Texture& outTexture);

#endif