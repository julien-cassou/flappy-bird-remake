#include <iostream>
#include <SFML/Graphics.hpp>
#include "bird.hpp"

/**
 * @brief Constructeur de l'oiseau
 * @param s Le type de personnage à charger (Sonic, TailS ou Knuckle)
 * @details Initialise l'oiseau avec sa position et charge la texture correspondante
 */
Bird::Bird(BirdType s) : position(X, Y), s(s) {
    if (s == BirdType::Sonic) {
        if (!texture.loadFromFile("assets/sonic.png"))
            throw std::runtime_error("Erreur de chargement de Sonic");
    } else if (s == BirdType::TailS) {
        if (!texture.loadFromFile("assets/tails.png"))
            throw std::runtime_error("Erreur de chargement de TailS");
    } else if (s == BirdType::Knuckle) {
        if (!texture.loadFromFile("assets/knuckle.png"))
            throw std::runtime_error("Erreur de chargement de Knuckle");
    }

    flappy.setTexture(texture);
    flappy.setPosition(position);
}

/**
 * @brief Récupère la position actuelle de l'oiseau
 * @return sf::Vector2f La position {x,y} de l'oiseau
 */
sf::Vector2f Bird::getPosition() const {
    return position;
}

/**
 * @brief Fait sauter l'oiseau
 * @details Réinitialise la vitesse et déplace l'oiseau vers le haut
 */
void Bird::saut() {
    vitesse = 0;
    position.y -= fY / 7.f;
    if (position.y < -150) position.y = -150;
    flappy.setPosition(position);
}

/**
 * @brief Met à jour la position de l'oiseau
 * @param dt Le temps écoulé depuis la dernière mise à jour
 * @details Applique la gravité et met à jour la position de l'oiseau
 */
void Bird::update(float dt) {
    vitesse += gravite * dt;
    position.y += vitesse * dt;
    flappy.setPosition(position);
}

/**
 * @brief Dessine l'oiseau dans la fenêtre
 * @return sf::Sprite Le sprite de l'oiseau à afficher
 */
sf::Sprite Bird::dessinerOiseau() {
    // flappy.setSize(sf::Vector2f(44.f, 35.f));
    flappy.setPosition(position);
    return flappy;
}

/**
 * @brief Réinitialise l'oiseau à sa position de départ
 * @details Remet l'oiseau à sa position initiale et réinitialise sa vitesse
 */
void Bird::reset() {
    position = sf::Vector2f (X, Y);
    vitesse = 0.f; 
}

/**
 * @brief Récupère la vitesse actuelle de l'oiseau
 * @return float La vitesse verticale de l'oiseau
 */
float Bird::getVelocity() const {
    return vitesse;
}

/**
 * @brief Modifie l'apparence de l'oiseau
 * @param s Le nouveau type de personnage à charger
 * @details Charge la nouvelle texture correspondant au personnage sélectionné
 */
void Bird::ModifieAffichage(BirdType s) {
    if (s == BirdType::Sonic) {
        if (!texture.loadFromFile("assets/sonic.png"))
            throw std::runtime_error("Erreur de chargement de Sonic");
    } else if (s == BirdType::TailS) {
        if (!texture.loadFromFile("assets/tails.png"))
            throw std::runtime_error("Erreur de chargement de TailS");
    } else if (s == BirdType::Knuckle) {
        if (!texture.loadFromFile("assets/knuckle.png"))
            throw std::runtime_error("Erreur de chargement de Knuckle");
    }
    flappy.setTexture(texture);
    flappy.setPosition(position);
}

/**
 * @brief Récupère le type BirdType de l'oiseau
 * @return s une variable de type BirdType
 */
BirdType Bird::getS() const {
    return s;
}

void Bird::setS(BirdType u) {
    s = u;
}