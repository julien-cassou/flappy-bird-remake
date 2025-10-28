#include <iostream>
#include "tuyaux.hpp"
#include <random>
#include <cmath>
#include <algorithm>

const float VITESSE_TUYAU = 150.f;
sf::Texture tuyau::haut;
sf::Texture tuyau::bas;

void tuyau::chargerTextures() {
    if (!haut.loadFromFile("assets/pillierHaut.png"))
        throw std::runtime_error("Erreur chargement pilier haut");
    if (!bas.loadFromFile("assets/pillierBas.png"))
        throw std::runtime_error("Erreur chargement pilier bas");
}

/**
 * @brief Constructeur d'un tuyau
 * @param enHaut Booléen indiquant si le tuyau est en haut (true) ou en bas (false)
 * @param hauteur Hauteur du tuyau en pixels
 * @details Initialise un tuyau avec sa position et ses dimensions selon qu'il soit en haut ou en bas
 */
tuyau::tuyau(bool enHaut, float hauteur) {
    const float largeurTuyau = 110.f;

    if (enHaut) {
        rectangle.setTexture(haut);
        int fullHeight = haut.getSize().y; // ex: 373
        int affichée = static_cast<int>(hauteur + 150.f - (fY - barrière)); // ex: 430

        // --- DÉBUT DE LA CORRECTION ---
        
        // Calcule le 'top' (peut être négatif)
        int rectTop = fullHeight - affichée; // ex: 373 - 430 = -57
        // Calcule la hauteur
        int rectHeight = affichée; // ex: 430

        // Si le 'top' est négatif, on le ramène à 0
        // et on réduit la hauteur d'autant.
        if (rectTop < 0) {
            rectHeight = rectHeight + rectTop; // ex: 430 + (-57) = 373
            rectTop = 0;
        }

        // Sécurité supplémentaire : la hauteur ne peut pas être négative
        if (rectHeight < 0) {
            rectHeight = 0;
        }

        // Utilise les valeurs corrigées
        rectangle.setTextureRect(sf::IntRect(0, rectTop, static_cast<int>(largeurTuyau), rectHeight));
        
        // --- FIN DE LA CORRECTION ---

        position = {fX + 500, -150.f};
    
    } else { // Le code pour le tuyau BAS semble correct
        rectangle.setTexture(bas);
        
        // On ajoute une sécurité pour la hauteur 0
        int rectHeight = static_cast<int>(hauteur);
        if (rectHeight < 0) rectHeight = 0; 
        
        rectangle.setTextureRect(sf::IntRect(0, 0, static_cast<int>(largeurTuyau), rectHeight));
        position = {fX + 500, barrière - hauteur};
    }
    rectangle.setPosition(position);
}


/**
 * @brief Fait avancer le tuyau vers la gauche
 * @details Déplace le tuyau de 0.05 unités vers la gauche à chaque appel
 */
void tuyau::avance(float dt) {
    position.x = position.x - (VITESSE_TUYAU * dt);
    rectangle.setPosition(position);
}

/**
 * @brief Récupère la forme rectangulaire du tuyau
 * @return sf::Sprite La forme du tuyau pour l'affichage
 */
sf::Sprite tuyau::getRectangle() const {
    return rectangle;
}

/**
 * @brief Récupère la position actuelle du tuyau
 * @return sf::Vector2f La position {x,y} du tuyau
 */
sf::Vector2f tuyau::getPosition() const {
    return position;
}

/**
 * @brief Génère aléatoirement les dimensions d'une paire de tuyaux
 * @details Crée une paire de tuyaux avec un espace entre eux pour le passage de l'oiseau
 */
void PairTuyaux::tailleRandom() {
    float trou = fY / 4.f;
    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_real_distribution<float> dist(100.f, fY - trou - 100.f); // pour éviter négatif

    float hauteurBas = dist(rng);
    float hauteurHaut = fY - hauteurBas - trou;

    t1 = tuyau{true, hauteurHaut};
    t2 = tuyau{false, hauteurBas};
}

/**
 * @brief Constructeur d'une paire de tuyaux
 * @details Initialise une paire de tuyaux avec des dimensions aléatoires
 */
PairTuyaux::PairTuyaux() : t1{true, 0}, t2{false, 0} {
    tailleRandom();
}

/**
 * @brief Vérifie si la paire de tuyaux est sortie de l'écran
 * @return bool True si les tuyaux sont sortis de l'écran, False sinon
 */
bool PairTuyaux::HorsEcran() const {
    return t1.getPosition().x < -110.f;
}

