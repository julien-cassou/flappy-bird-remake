#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "menu.hpp"
#include <list>
#include <fstream>

/**
 * @brief Crée un texte "Game Over" avec les instructions pour recommencer
 * @return sf::Text Le texte formaté pour l'affichage du Game Over
 */
sf::Text GameOver() {
    sf::Text text;
    text.setCharacterSize(36);
    text.setFillColor(sf::Color::Black);
    text.setString("- Game Over - \n appuie sur R pour recommencer");
    text.setPosition(X - 200,Y);
    return text;
}

/**
 * @brief lie un fichier qui stocke le score le plus élevé
 * @return renvoie le score le plus élevé
 */
int HighScore(const std::string& fichier) {
    std::ifstream file(fichier);
    int highscore = 0;
    if (file.is_open()) {
        file >> highscore;
        file.close();
    }
    return highscore;
}


void SetHighScore(const std::string& fichier, int score) {
    std::ofstream file(fichier);
    if (file.is_open()) {
        file << score;
        file.close();
    }
    else {
        std::cerr << "Erreur ouverture de HighScore";
    }
}   

/**
 * @brief Met à jour le score du joueur quand l'oiseau passe un tuyau
 * @details Incrémente le score et avance l'indice quand l'oiseau dépasse un tuyau
 */
void Jeu::MiseAJourScore() {
    if (l.empty() || indice == l.end()) return;

    if (oiseau.getPosition().x > indice->getT1().getPosition().x) {
        score++;
        ++indice;
    }
}

/**
 * @brief Réinitialise le score à 0
 */
void Jeu::ScoreRestart() {
    score = 0;
}

/**
 * @brief Crée et gère la boucle principale du jeu
 * @param window La fenêtre SFML où le jeu est affiché
 * @details Gère l'initialisation des ressources, la boucle de jeu, les événements,
 *          les collisions, l'affichage et la mise à jour du score
 */
void Jeu::creeJeu(sf::RenderWindow &window) {
    int Highest = HighScore("highscore.txt");
    bool jeuCommence = false;
    bool mort = false;
    bool freeze = false;
    float TempsDeFreeze = 0.f;
    float TempsTuyau = 0;
    sf::Clock TimerFond;
    sf::Clock timer; // utilisation d'un timer pour avoir le temps écoulé entre chaque frame

    sf::Text Gameover;

    int i = 0; // permet de changer le fond à chaque frame
    std::vector<sf::Texture> TextureFond(4);
    if (!TextureFond[0].loadFromFile("assets/img1.png")) throw std::runtime_error("Erreur fond1");
    if (!TextureFond[1].loadFromFile("assets/img2.png")) throw std::runtime_error("Erreur fond2");
    if (!TextureFond[2].loadFromFile("assets/img3.png")) throw std::runtime_error("Erreur fond3");
    if (!TextureFond[3].loadFromFile("assets/img4.png")) throw std::runtime_error("Erreur fond4");

    sf::Sprite fond;

    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf")) {
        throw std::runtime_error("La police ne se charge pas");
    }

    // affichage score
    sf::Text Score;
    Score.setCharacterSize(46);
    Score.setFillColor(sf::Color::White);
    Score.setOutlineColor(sf::Color::Black);
    Score.setOutlineThickness(5);
    Score.setFont(font);
    ScoreRestart();
    Score.setString(std::to_string(score));
    Score.setPosition({fX / 2, 50.f});

    // délimitation du sol
    sf::RectangleShape sol;
    sol.setSize(sf::Vector2f(fX, 10.f));
    sol.setPosition(0.f, barrière);
    sol.setFillColor(sf::Color::Transparent);

    // sprite de mort
    sf::Texture textureMort;
    switch(oiseau.getS()) {
        case BirdType::Sonic: 
            if(!textureMort.loadFromFile("assets/mort/sonic.png")) {
                throw std::runtime_error("mort Sonic");
            }
           break;
        case BirdType::TailS: 
            if(!textureMort.loadFromFile("assets/mort/tails.png")) {
                throw std::runtime_error("Mort Tails");
            }
            break;
        case BirdType::Knuckle: 
            if(!textureMort.loadFromFile("assets/mort/knuckle.png")) {
                throw std::runtime_error("Mort Knuckle");
            }
            break;
    }
    int muerto;
    bool AfficheScore = false;

    // boutons de mort
    sf::Texture t1;
    if (!t1.loadFromFile("assets/mort/restart.png")) throw std::runtime_error("t1");
    sf::Texture t2;
    if (!t2.loadFromFile("assets/mort/menu.png")) throw std::runtime_error("t2");
    sf::Texture t3;
    if (!t3.loadFromFile("assets/mort/affichagescore.png")) throw std::runtime_error("t3");
    sf::Texture t4;
    if (!t4.loadFromFile("assets/mort/continue.png")) throw std::runtime_error("t4");
    
    sf::Sprite b1;
    b1.setTexture(t1);
    b1.setPosition(fX /3 + 10, (fY / 4) * 2);
    sf::Sprite b2;
    b2.setTexture(t2);
    b2.setPosition(fX /3 + 10, (fY / 4) * 2 + 80);
    sf::Sprite b3;
    b3.setTexture(t3);
    b3.setPosition(fX /3 + 50, fY / 4);
    sf::Sprite b4;
    b4.setTexture(t4);
    b4.setPosition(fX /3 + 10, (fY / 4) * 2 - 10);

    // Texte des scores après la mort
    sf::Text ScoreMort;
    ScoreMort.setCharacterSize(36);
    ScoreMort.setFillColor(sf::Color::White);
    ScoreMort.setFont(font);
    ScoreMort.setPosition(fX /3 + 93, fY / 4 + 35);
    sf::Text HighScoreMort;
    HighScoreMort.setCharacterSize(36);
    HighScoreMort.setFillColor(sf::Color::White);
    HighScoreMort.setFont(font);
    HighScoreMort.setPosition(fX /3 + 93, fY / 4 + 95);

    // Chargement sounboard
    sf::SoundBuffer mortBuffer;
    mortBuffer.loadFromFile("assets/sound/sonicded.wav");
    sf::Sound death;
    death.setBuffer(mortBuffer);

    sf::SoundBuffer GAMEOVERBuffer;
    GAMEOVERBuffer.loadFromFile("assets/sound/gameover.wav");
    sf::Sound GAMEOVER;
    GAMEOVER.setBuffer(GAMEOVERBuffer);
    bool gameoverplayed = false;

    sf::SoundBuffer jumpBuffer;
    jumpBuffer.loadFromFile("assets/sound/jump.wav");
    sf::Sound jump;
    jump.setBuffer(jumpBuffer);

    while (window.isOpen()) {
        float dt = timer.restart().asSeconds(); // ici, dt permet ainsi de rendre la chute plus fluide
        TempsTuyau += dt;
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i sourisPixel = sf::Mouse::getPosition(window);
                    sf::Vector2f souris = window.mapPixelToCoords(sourisPixel);
                    if (b1.getGlobalBounds().contains(souris) && mort) {
                        GAMEOVER.stop();
                        gameoverplayed = false;
                        mort = false;
                        freeze = false;
                        oiseau.reset();
                        l.clear();
                        ScoreRestart();
                        jeuCommence = false;
                        AfficheScore = false;
                    }
                    else if (b2.getGlobalBounds().contains(souris)) {
                        Menu menu;
                        sf::RenderWindow fenetre(sf::VideoMode(TailleX, TailleY), "Main Menu");
                        GAMEOVER.stop();
                        window.close();
                        menu.MainMenu(fenetre);
                    }
                    else if (b4.getGlobalBounds().contains(souris) && freeze) {
                        freeze = false;
                    }
                }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape && !mort) {
                    freeze = !freeze;
                }
                if (event.key.code == sf::Keyboard::R) {
                    GAMEOVER.stop();
                    gameoverplayed = false;
                    mort = false;
                    freeze = false;
                    oiseau.reset();
                    l.clear();
                    ScoreRestart();
                    jeuCommence = false;
                    AfficheScore = false;
                }
                if (!mort && event.key.code == sf::Keyboard::Space && !freeze) {
                    oiseau.saut();
                    if (jeuCommence == false) { // enlève le bug des deux tuyaux au début
                        jump.play();
                        TempsTuyau = 0;
                    }
                    jeuCommence = true;
                }
            }
        }

        if (jeuCommence && !mort && !freeze) {
            oiseau.update(dt);
            TempsTuyau -= TempsDeFreeze;
            creeTuyaux(TempsTuyau, TempsDeFreeze, dt);
            MiseAJourScore();
            Score.setString(std::to_string(score));
        }

        window.clear(sf::Color::White);
        fond.setTexture(TextureFond[i]);
        if (TimerFond.getElapsedTime().asSeconds() > 1) {
            i++; 
            TimerFond.restart();
        }
        if (i > 3) i = 0;
        window.draw(fond);

        sf::Sprite rec = oiseau.dessinerOiseau();
        if (mort) rec.setTexture(textureMort);
        if (rec.getGlobalBounds().intersects(sol.getGlobalBounds()) && !mort) {
            mort = true; // flappy meurt en cas de collision avec le sol
            death.play();
            freeze = false;
            if (score > Highest) {
                SetHighScore("highscore.txt", score);
                Highest = score;
            }
            muerto = 0;
        }
        // gère l'intersection entre flappy et tuyau en prédisant son next mouv
        sf::FloatRect futureBounds = rec.getGlobalBounds();
        futureBounds.top += oiseau.getVelocity() * dt;
        for (auto& t: l) {
            if((futureBounds.intersects(t.getT1().getRectangle().getGlobalBounds()) || 
            futureBounds.intersects(t.getT2().getRectangle().getGlobalBounds())) && !mort) {
                mort = true;
                death.play();
                freeze = false;
                if (score > Highest) {
                    SetHighScore("highscore.txt", score);
                    Highest = score;
                }
                muerto = 0;
            }
        }
        window.draw(rec);
        window.draw(sol);
        for (auto& t : l) {
            window.draw(t.getT1().getRectangle());
            window.draw(t.getT2().getRectangle());
        }
        if (!AfficheScore) window.draw(Score);
        if (mort) {
            sf::Vector2f pos = oiseau.getPosition();
            if (pos.y > fY + 200) {
                if (!gameoverplayed) {
                    GAMEOVER.play();
                    gameoverplayed = true;
                }
                HighScoreMort.setString(std::to_string(Highest));
                ScoreMort.setString(std::to_string(score));

                window.draw(b1);
                window.draw(b2);
                window.draw(b3);
                window.draw(HighScoreMort);
                window.draw(ScoreMort);
                AfficheScore = true;
            } else {
                if (muerto == 0) {
                    oiseau.saut(); 
                    muerto++;
                }
                else {
                    oiseau.update(dt);
                }
            }
        }  
        if (freeze) {
            TempsDeFreeze += dt;

            HighScoreMort.setString(std::to_string(Highest));
            ScoreMort.setString(std::to_string(score));
            window.draw(b4);
            window.draw(b2);
            window.draw(b3);
            window.draw(HighScoreMort);
            window.draw(ScoreMort);
        }
        window.display();
    }
}

/**
 * @brief Crée et gère les tuyaux du jeu
 * @param TempTuyau, le temps écoulée via Horloge SFML pour réguler l'apparition des tuyaux
 * @param temps de freeze que l'on va juste réinitialiser  
 * @details Crée de nouvelles paires de tuyaux à intervalles réguliers,
 *          les fait avancer et supprime ceux qui sortent de l'écran
 */
void Jeu::creeTuyaux(float &TempsTuyau, float &temps, float dt) {
    if (l.empty()) {
        l.push_back(PairTuyaux());
        indice = l.begin();
    }

    if (TempsTuyau > 2.2 ) {
        l.push_back(PairTuyaux());
        TempsTuyau = 0;

        // si c'est le premier tuyau ajouté (juste au cas où)
        if (l.size() == 1) {
            indice = l.begin();
        }
    }

    while (!l.empty() && l.front().HorsEcran()) {
        if (indice == l.begin()) {
            // on supprime l'élément pointé → il faut réinitialiser `indice`
            l.pop_front();
            indice = l.begin();
        } else {
            // sinon on est sûr que `indice` n'est pas invalidé
            l.pop_front();
        }
    }

    for (auto& t : l) {
        t.getT1().avance(dt);
        t.getT2().avance(dt);
    }

    temps = 0;
}

/**
 * @brief Configure et affiche le menu des options
 * @param window La fenêtre SFML où le menu est affiché
 * @details Permet de choisir le personnage jouable et de lancer le jeu
 */
void Jeu::setOption(sf::RenderWindow &window) {
    sf::Texture texture;
    if (!texture.loadFromFile("assets/menu/MenuFond.png")) throw std::runtime_error("fond ne se charge pas");
    sf::Sprite fond;
    fond.setTexture(texture);

    sf::Texture fleche1;
    if(!fleche1.loadFromFile("assets/menu/fleche1.png")) throw std::runtime_error("f1 ne se charge pas");
    sf::Sprite flèche1;
    flèche1.setTexture(fleche1);
    flèche1.setPosition(10, TailleY/2  -75);

    sf::Texture fleche2;
    if(!fleche2.loadFromFile("assets/menu/fleche2.png")) throw std::runtime_error("f2 ne se charge pas");
    sf::Sprite flèche2;
    flèche2.setTexture(fleche2);
    flèche2.setPosition(TailleX - 100, TailleY/2 - 75);

    sf::Texture TextureBouton1;
    if (!TextureBouton1.loadFromFile("assets/menu/play.png")) throw std::runtime_error("Erreur bouton1");
    sf::Sprite bouton1;
    bouton1.setPosition(TailleX/2 - 50 , 350);
    bouton1.setTexture(TextureBouton1);

    std::vector<sf::Texture> OptionTexture (3);
    if (!OptionTexture[0].loadFromFile("assets/options/Sonic.png"))
        throw std::runtime_error("Erreur de chargement de Sonic options");
    if (!OptionTexture[1].loadFromFile("assets/options/TailS.png"))
        throw std::runtime_error("Erreur de chargement de TailS options");
    if (!OptionTexture[2].loadFromFile("assets/options/Knuckle.png"))
        throw std::runtime_error("Erreur de chargement de Knuckle options");

    sf::Sprite bird;
    bird.setPosition(TailleX /2 - 300/2, TailleY /2 - 270/2 );
    bird.setTexture(OptionTexture[0]);
    int i = 0;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                else if (event.key.code == sf::Keyboard::Left) {
                    i = (i - 1 + 3) % 3; // pour boucler entre 0 et 1
                    bird.setTexture(OptionTexture[i]);
                    oiseau.setS(static_cast<BirdType>(i));
                    oiseau.ModifieAffichage(static_cast<BirdType>(i));
                } else if (event.key.code == sf::Keyboard::Right) {
                    i = (i + 1) % 3;
                    bird.setTexture(OptionTexture[i]);
                    oiseau.setS(static_cast<BirdType>(i));
                    oiseau.ModifieAffichage(static_cast<BirdType>(i));
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i sourisPixel = sf::Mouse::getPosition(window);
                sf::Vector2f souris = window.mapPixelToCoords(sourisPixel);
                if (bouton1.getGlobalBounds().contains(souris)) {
                    sf::RenderWindow fenetreJeu(sf::VideoMode(fX, fY), "FLAPPY THE BIRD");
                    window.close();
                    creeJeu(fenetreJeu);
                }
                if (flèche1.getGlobalBounds().contains(souris)) {
                    i = (i - 1 + 3) % 2;
                    bird.setTexture(OptionTexture[i]);
                    oiseau.setS(static_cast<BirdType>(i));
                    oiseau.ModifieAffichage(static_cast<BirdType>(i));
                } else if (flèche2.getGlobalBounds().contains(souris)) {
                    i = (i + 1) % 3;
                    bird.setTexture(OptionTexture[i]);
                    oiseau.setS(static_cast<BirdType>(i));
                    oiseau.ModifieAffichage(static_cast<BirdType>(i));
                }
            }

            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                sf::RenderWindow fenetreJeu(sf::VideoMode(fX, fY), "FLAPPY THE BIRD");
                window.close();
                creeJeu(fenetreJeu);
            }
        }
    window.clear(sf::Color::Black);
    window.draw(fond);
    window.draw(bird);
    window.draw(flèche1);
    window.draw(flèche2);
    window.draw(bouton1);
    window.display();
    }
}
