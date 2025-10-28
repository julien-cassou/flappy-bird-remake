#ifndef JEU_HPP
#define JEU_HPP
#include "tuyaux.hpp"
#include <SFML/System/Vector2.hpp>
#include <list>

class Jeu {
    private:
        Bird oiseau;
        std::list<PairTuyaux> l;
        int score;
        std::list<PairTuyaux>::iterator indice = l.end();
    public: 
        //constructeur
        Jeu(BirdType s) : oiseau{s}, l{}, score{0} {}
        //méthodes
        void creeJeu(sf::RenderWindow &window);
        void creeTuyaux(float &TempsTuyau, float &temps, float dt);
        void MiseAJourScore();
        void ScoreRestart();
        void setOption(sf::RenderWindow &window);
};

sf::Text GameOver();
int HighScore(const std::string& fichier);
void SetHighScore(const std::string& fichier, int score);
#endif