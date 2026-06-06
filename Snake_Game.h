#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Snake_Game
{
private:
    static const int REDAK = 20;
    static const int STUPAC = 40;

    sf::Font font;

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

    bool bool_raste;

    std::vector<sf::Vector2i> put;
    std::vector<sf::Vector2i> zmija;
    std::vector<sf::Vector2i> voce;

    bool izvan(int red, int stupac);

    void generirajZmiju();
    void generirajZidove();
    void generirajA_i_B();

    void izracunajPut();

    void odrediCilj(int& red, int& stupac);

public:
    Snake_Game();

    void update();
    void iscrtaj(sf::RenderWindow& window);

    void postaviVoce(int red, int stupac);
    bool postavljanje_voca;
    int broj_postavljenog_voca;

    void izvrsiPostavljanje(sf::Vector2i pozicija_misa);
};