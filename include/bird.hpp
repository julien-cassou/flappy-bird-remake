#ifndef BIRD_HPP
#define BIRD_HPP
#include <iostream>
#include <SFML/Graphics.hpp>


const float gravite = 1000.f;
const float fY = 760.f;
const float fX = 605.f;
const int Y = fY / 2.f;
const int X = fX / 2.f - 50.f;

const float TailleX = 517.f;
const float TailleY = 418.f;

const float barrière = fY - 100;

// couleur utilisé ensuite pr la séléction du flappy
enum class BirdType {Sonic, TailS, Knuckle};

class Bird {
    private:
        sf::Vector2f position;  //stockage de ses coordonnées {x,y}
        sf::Sprite flappy;
        sf::Texture texture;
        BirdType s;
        float vitesse = 0.f;  //vitesse de chute

    public:
        // constucteur
        Bird(BirdType s);

        //getters
        sf::Vector2f getPosition() const;
        float getVelocity() const;
        BirdType getS() const;

        // les mises à jour
        void saut();
        void update(float dt);
        sf::Sprite dessinerOiseau();
        void reset();
        void ModifieAffichage(BirdType s);
        void setS(BirdType u);
};
#endif