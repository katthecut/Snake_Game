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

    int a_redak, a_stupac;
    int b_redak, b_stupac;

    std::vector<sf::Vector2i> put;
    std::vector<sf::Vector2i> zmija;
    std::vector<sf::Vector2i> voce;

    bool izvan(int r, int s);
    void izracunajPut();
    void generirajZidove();
    void generirajVoce();
    bool idi_na_voce;
    void odrediCilj(int& cilj_redak, int& cilj_stupac);

public:
    Snake_Game();

    void update();
    void iscrtaj(sf::RenderWindow& window);
};