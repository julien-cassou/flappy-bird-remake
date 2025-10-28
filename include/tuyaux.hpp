#ifndef TUYAUX_HPP
#define TUYAUX_HPP
#include "bird.hpp"
#include <SFML/Graphics.hpp>

    class tuyau {
        private:
            sf::Sprite rectangle; //rectangle SFML
            sf::Vector2f position; //position {x,y}
            static sf::Texture haut;
            static sf::Texture bas;
        public:
            //Constructeur
            tuyau(bool enHaut, float hauteur);

            //getters
            sf::Sprite getRectangle() const;
            sf::Vector2f getPosition() const;
            // méthodes
            void avance(float dt);
            static void chargerTextures();
    };

    class PairTuyaux {
        private:
            tuyau t1;
            tuyau t2;
        public: 
            //constructeur
            PairTuyaux();

            void tailleRandom(); // créer la paire de tuyaux
            tuyau& getT1() {return t1;};
            tuyau& getT2() {return t2;};
            bool HorsEcran() const; // permet de déterminer si le couple de tuyaux sort de l'écran
    };
    #endif

