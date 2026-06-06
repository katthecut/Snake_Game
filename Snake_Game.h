#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Snake_Game
{
private:
    static const int REDAK = 20;
    static const int STUPAC = 40;

    enum Polje
    {
        PRAZNO,
        ZID,
        A,
        B,
        VOCE
    };

    Polje mapa[REDAK][STUPAC];

    int a_red;
    int a_stupac;

    int b_red;
    int b_stupac;

    int faza;

    int prvo_voce_index;
    int drugo_voce_index;

    std::vector<sf::Vector2i> put;
    std::vector<sf::Vector2i> zmija;
    std::vector<sf::Vector2i> voce;

    bool izvan(int red, int stupac);

    void generirajZmiju();
    void generirajZidove();
    void generirajA_i_B();
    void generirajVoce();
    void izracunajPut();

    void odrediCilj(int& red, int& stupac);

public:
    Snake_Game();

    void update();
    void iscrtaj(sf::RenderWindow& window);
};