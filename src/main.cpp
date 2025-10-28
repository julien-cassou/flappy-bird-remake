#include <iostream>
#include "menu.hpp"

int main() {
    tuyau::chargerTextures();
    Menu menu;
    sf::RenderWindow window(sf::VideoMode(TailleX, TailleY), "Main Menu");
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(140);
    menu.MainMenu(window);
    return 0;
}