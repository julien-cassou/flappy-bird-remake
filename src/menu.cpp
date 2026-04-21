#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "menu.hpp"

/**
 * @brief Crée et configure le titre du jeu
 * @param outTexture Texture de sortie qui contiendra le titre
 * @return sf::Sprite Le sprite du titre à afficher
 * @details Combine deux images pour créer le titre du jeu et le centre dans la fenêtre
 */
sf::Sprite setTitle(sf::Texture& outTexture) {
    sf::Texture t1, t2;

    if (!t1.loadFromFile("assets/menu/title1.png"))
        throw std::runtime_error("title1");
    if (!t2.loadFromFile("assets/menu/title2.png"))
        throw std::runtime_error("title2");

    sf::Sprite s1(t1);
    sf::Sprite s2(t2);

    sf::FloatRect bounds1 = s1.getLocalBounds();
    sf::FloatRect bounds2 = s2.getLocalBounds();

    float largeur = std::max(bounds1.width, bounds2.width);
    float hauteur = bounds1.height + bounds2.height + 10; // marge verticale

    // Position dans la texture, PAS sur l'écran
    s1.setPosition((largeur - bounds1.width) / 2, 0);
    s2.setPosition((largeur - bounds2.width) / 2, bounds1.height + 10);

    sf::RenderTexture renderTexture;
    renderTexture.create(static_cast<unsigned int>(largeur), static_cast<unsigned int>(hauteur));
    renderTexture.clear(sf::Color::Transparent);
    renderTexture.draw(s1);
    renderTexture.draw(s2);
    renderTexture.display();

    outTexture = renderTexture.getTexture(); // copie vers texture externe
    sf::Sprite title(outTexture);

    // Centrage final sur la fenêtre
    title.setPosition((TailleX - largeur) / 2.f, TailleY / 8.f);

    return title;
}

/**
 * @brief Affiche et gère le menu principal du jeu
 * @param window La fenêtre SFML où le menu est affiché
 * @details Gère l'affichage du menu principal avec les boutons Play et Options,
 *          et les interactions utilisateur pour naviguer dans le menu
 */
void Menu::MainMenu(sf::RenderWindow &window) {
    sf::Texture texture;
    if (!texture.loadFromFile("assets/menu/MenuFond.png")) throw std::runtime_error("fond ne se charge pas");
    sf::Sprite fond;
    fond.setTexture(texture);

    sf::Texture TextureBouton1;
    if (!TextureBouton1.loadFromFile("assets/menu/play.png")) throw std::runtime_error("Erreur bouton1");

    sf::Texture TextureBouton2;
    if (!TextureBouton2.loadFromFile("assets/menu/options.png")) throw std::runtime_error("Erreur bouton2");


    sf::Sprite bouton1;
    bouton1.setPosition({TailleX/3 - 40.f, (TailleY/2) + 20.f});
    bouton1.setTexture(TextureBouton1);
    sf::Sprite bouton2;
    bouton2.setPosition({2*(TailleX/4) , (TailleY/2) + 20.f});
    bouton2.setTexture(TextureBouton2);

    sf::Texture out;
    sf::Sprite Title = setTitle(out);

    sf::SoundBuffer openingBuffer;
    openingBuffer.loadFromFile("assets/sound/01-opening.wav");
    sf::Sound opening;
    opening.setBuffer(openingBuffer);
    opening.play();

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i sourisPixel = sf::Mouse::getPosition(window);
                sf::Vector2f souris = window.mapPixelToCoords(sourisPixel);
                if (bouton1.getGlobalBounds().contains(souris)) {
                    opening.stop();
                    sf::RenderWindow fenetreJeu(sf::VideoMode(fX, fY), "FLAPPY THE BIRD");
                    window.close();
                    jeu.creeJeu(fenetreJeu);
                }
                if (bouton2.getGlobalBounds().contains(souris)) {
                    sf::RenderWindow options(sf::VideoMode(TailleX, TailleY), "OPTIONS");
                    window.close();
                    opening.stop();
                    jeu.setOption(options);
                }
            }
        }
    window.draw(fond);
    window.draw(Title);
    window.draw(bouton1);
    window.draw(bouton2);
    window.display();
    }
}

