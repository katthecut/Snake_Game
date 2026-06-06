#include <SFML/Graphics.hpp>
#include "Snake_Game.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

//provjera izlaska iz mape
bool Snake_Game::izvan(int red, int stupac)
{
    return (red < 0 || red >= REDAK || stupac < 0 || stupac >= STUPAC);
}

Snake_Game::Snake_Game()
{
    postavljanje_voca = true;
    broj_postavljenog_voca = 0;

    srand(time(nullptr));

    if (!font.loadFromFile("editundo.ttf"))
    {
        cout << "Font nije ucitan" << endl;
    }

    for (int red = 0; red < REDAK; red++)
    {
        for (int stupac = 0; stupac < STUPAC; stupac++)
        {
            mapa[red][stupac] = PRAZNO;
        }
    }

    faza = 0;
    bool_raste = false;

    generirajZmiju();
    generirajZidove();
    generirajA_i_B();

    izracunajPut();
}

//zmija je na pocetku uvijek duljine 3
void Snake_Game::generirajZmiju()
{
    zmija.clear();

    while (true)
    {
        int red_glava = rand() % REDAK;
        int stupac_glava = rand() % STUPAC;

        int smjer = rand() % 4;

        int pomak_red = 0;
        int pomak_stupac = 0;

        if (smjer == 0) pomak_red = 1;
        if (smjer == 1) pomak_red = -1;
        if (smjer == 2) pomak_stupac = 1;
        if (smjer == 3) pomak_stupac = -1;

        int red_drugi = red_glava + pomak_red;
        int stupac_drugi = stupac_glava + pomak_stupac;

        int red_treci = red_glava + 2 * pomak_red;
        int stupac_treci = stupac_glava + 2 * pomak_stupac;

        if (izvan(red_glava, stupac_glava)) continue;
        if (izvan(red_drugi, stupac_drugi)) continue;
        if (izvan(red_treci, stupac_treci)) continue;

        zmija.push_back({ red_glava, stupac_glava });
        zmija.push_back({ red_drugi, stupac_drugi });
        zmija.push_back({ red_treci, stupac_treci });

        break;
    }
}

void Snake_Game::generirajZidove()
{
    for (int red = 0; red < REDAK; red++)
    {
        for (int stupac = 0; stupac < STUPAC; stupac++)
        {
            if (mapa[red][stupac] != A && mapa[red][stupac] != B)
                mapa[red][stupac] = PRAZNO;
        }
    }

    const int GAP = 2;
    const int DENSITY = 5;

    for (int redak = 1; redak < REDAK - 1; redak++)
    {
        for (int stupac = 1; stupac < STUPAC - 1; stupac++)
        {
            if (mapa[redak][stupac] != PRAZNO)
                continue;

            if (rand() % 100 < DENSITY)
            {
                int duljina = 2 + rand() % 4;

                bool moze = true;

                for (int k = -GAP; k < duljina + GAP; k++)
                {
                    int novi_stupac = stupac + k;

                    if (izvan(redak, novi_stupac))
                        continue;

                    if (mapa[redak][novi_stupac] == ZID)
                    {
                        moze = false;
                        break;
                    }
                }

                if (!moze)
                    continue;

                for (int k = 0; k < duljina; k++)
                {
                    int novi_stupac = stupac + k;

                    if (!izvan(redak, novi_stupac))
                        mapa[redak][novi_stupac] = ZID;
                }
            }
        }
    }

    for (int redak = 1; redak < REDAK - 1; redak++)
    {
        for (int stupac = 1; stupac < STUPAC - 1; stupac++)
        {
            if (mapa[redak][stupac] != PRAZNO)
                continue;

            if (rand() % 100 < DENSITY)
            {
                int duljina = 2 + rand() % 4;

                bool moze = true;

                for (int k = -GAP; k < duljina + GAP; k++)
                {
                    int novi_redak = redak + k;

                    if (izvan(novi_redak, stupac))
                        continue;

                    if (mapa[novi_redak][stupac] == ZID)
                    {
                        moze = false;
                        break;
                    }
                }

                if (!moze)
                    continue;

                for (int k = 0; k < duljina; k++)
                {
                    int novi_redak = redak + k;

                    if (!izvan(novi_redak, stupac))
                        mapa[novi_redak][stupac] = ZID;
                }
            }
        }
    }
}

void Snake_Game::generirajA_i_B()
{
    a_red = zmija.front().x;
    a_stupac = zmija.front().y;

    while (true)
    {
        b_red = rand() % REDAK;
        b_stupac = rand() % STUPAC;

        if (b_red == a_red && b_stupac == a_stupac)
            continue;

        break;
    }

    mapa[b_red][b_stupac] = B;
}

void Snake_Game::postaviVoce(int red, int stupac)
{
    if (!postavljanje_voca)
        return;

    if (izvan(red, stupac))
        return;

    //if (mapa[red][stupac] == ZID || B || VOCE) return;
    
    if (mapa[red][stupac] == ZID) 
        return;
    if (mapa[red][stupac] == B)
        return;
    if (mapa[red][stupac] == VOCE)
        return;


    for (int i = 0; i < zmija.size(); i++)
    {
        if (zmija[i].x == red && zmija[i].y == stupac)
            return;
    }

    voce.push_back({ red, stupac });
    mapa[red][stupac] = VOCE;

    broj_postavljenog_voca++;

    if (broj_postavljenog_voca >= 3)
    {
        postavljanje_voca = false;
        izracunajPut();
    }

}

void Snake_Game::izvrsiPostavljanje(sf::Vector2i pozicija_misa)
{
    const int velicina = 20;
    const int offset_y = 40;

    int x = pozicija_misa.x;
    int y = pozicija_misa.y;
    y -= offset_y;

    if (y < 0)
        return;

    int stupac = x / velicina;
    int red = y / velicina;

    if (red < 0 || red >= REDAK) return;
    if (stupac < 0 || stupac >= STUPAC) return;

    postaviVoce(red, stupac);
}
void Snake_Game::izracunajPut()
{
    if (postavljanje_voca)
        return;

    struct Node
    {
        int red;
        int stupac;
    };

    bool posjeceno[REDAK][STUPAC] = { false };
    sf::Vector2i prethodni[REDAK][STUPAC];

    std::queue<Node> red_cekanja;

    int udaljenost[REDAK][STUPAC];

    const int max_broj_koraka = 1000000;

    for (int red = 0; red < REDAK; red++)
    {
        for (int stupac = 0; stupac < STUPAC; stupac++)
        {
            udaljenost[red][stupac] = max_broj_koraka;
            prethodni[red][stupac] = sf::Vector2i(-1, -1);
        }
    }

    int red_pocetak = zmija.front().x;
    int stupac_pocetak = zmija.front().y;

    int cilj_red;
    int cilj_stupac;

    odrediCilj(cilj_red, cilj_stupac);

    udaljenost[red_pocetak][stupac_pocetak] = 0;
    red_cekanja.push({ red_pocetak, stupac_pocetak });

    int pomak_red[4] = { -1, 1, 0, 0 };
    int pomak_stupac[4] = { 0, 0, -1, 1 };

    while (!red_cekanja.empty())
    {
        Node trenutni = red_cekanja.front();
        red_cekanja.pop();

        if (posjeceno[trenutni.red][trenutni.stupac])
            continue;

        posjeceno[trenutni.red][trenutni.stupac] = true;

        for (int i = 0; i < 4; i++)
        {
            int novi_red = trenutni.red + pomak_red[i];
            int novi_stupac = trenutni.stupac + pomak_stupac[i];

            if (izvan(novi_red, novi_stupac))
                continue;

            if (mapa[novi_red][novi_stupac] == ZID)
                continue;

            bool polje_je_tijelo_zmije = false;

            for (int indeks = 0; indeks < zmija.size(); indeks++)
            {
                if (zmija[indeks].x == novi_red &&
                    zmija[indeks].y == novi_stupac)
                {
                    polje_je_tijelo_zmije = true;
                    break;
                }
            }

            if (polje_je_tijelo_zmije)
                continue;

            if (udaljenost[novi_red][novi_stupac] ==
                max_broj_koraka)
            {
                udaljenost[novi_red][novi_stupac] =
                    udaljenost[trenutni.red][trenutni.stupac] + 1;

                prethodni[novi_red][novi_stupac] =
                    sf::Vector2i(trenutni.red, trenutni.stupac);

                red_cekanja.push({ novi_red, novi_stupac });
            }
        }
    }

    put.clear();

    if (udaljenost[cilj_red][cilj_stupac] == max_broj_koraka)
        return;

    sf::Vector2i trenutni(cilj_red, cilj_stupac);

    while (!(trenutni.x == red_pocetak && trenutni.y == stupac_pocetak))
    {
        put.push_back(trenutni);

        sf::Vector2i prethodni_cvor = prethodni[trenutni.x][trenutni.y];

        if (prethodni_cvor.x == -1)
            break;

        trenutni = prethodni_cvor;
    }

    std::reverse(put.begin(), put.end());
}

void Snake_Game::odrediCilj(int& cilj_red, int& cilj_stupac)
{
    if (voce.size() == 0)
    {
        cilj_red = b_red;
        cilj_stupac = b_stupac;
        return;
    }

    int glava_red = zmija.front().x;
    int glava_stupac = zmija.front().y;

    int najblize_voce = 0;
    int max_broj_koraka = 1000000;

    for (int indeks = 0; indeks < voce.size(); indeks++)
    {
        int razlika_red = voce[indeks].x - glava_red;
        if (razlika_red < 0)
            razlika_red = -razlika_red;

        int razlika_stupac = voce[indeks].y - glava_stupac;
        if (razlika_stupac < 0)
            razlika_stupac = -razlika_stupac;

        int udaljenost = razlika_red + razlika_stupac;

        if (udaljenost < max_broj_koraka)
        {
            max_broj_koraka = udaljenost;
            najblize_voce = indeks;
        }
    }

    cilj_red = voce[najblize_voce].x;
    cilj_stupac = voce[najblize_voce].y;
}

void Snake_Game::update()
{
    if (postavljanje_voca)
        return;

    if (put.empty())
        return;

    sf::Vector2i nova_glava = put.front();
    put.erase(put.begin());
    
    for (int i = 0; i < zmija.size(); i++)
    {
        if (zmija[i].x == nova_glava.x &&
            zmija[i].y == nova_glava.y)
        {
            return;
        }
    }

    zmija.insert(zmija.begin(), nova_glava);

    if (!bool_raste)
    {
        sf::Vector2i stari_rep = zmija.back();
        zmija.pop_back();

        if (mapa[stari_rep.x][stari_rep.y] != ZID)
            mapa[stari_rep.x][stari_rep.y] = PRAZNO;
    }
    else
    {
        bool_raste = false;
    }

    if (mapa[nova_glava.x][nova_glava.y] == VOCE)
    {
        bool_raste = true;

        for (int i = 0; i < voce.size(); i++)
        {
            if (voce[i].x == nova_glava.x && voce[i].y == nova_glava.y)
            {
                voce.erase(voce.begin() + i);
                break;
            }
        }

        mapa[nova_glava.x][nova_glava.y] = PRAZNO;

        izracunajPut();
        return;
    }
}

void Snake_Game::iscrtaj(sf::RenderWindow& window)
{
    int velicina = 20;
    int offset_y = 40;

    sf::RectangleShape traka(sf::Vector2f(800.f, offset_y));
    traka.setFillColor(sf::Color::White);
    traka.setPosition(0.f, 0.f);
    window.draw(traka);

    sf::Text tekst;
    tekst.setFont(font);
    tekst.setCharacterSize(25);
    tekst.setFillColor(sf::Color::Black);

    if (postavljanje_voca)
        tekst.setString("Postavite 3 vocke");
    else
        tekst.setString("");

    tekst.setPosition(10.f, 10.f);
    window.draw(tekst);

    sf::RectangleShape celija(sf::Vector2f(20.f, 20.f));

    for (int red = 0; red < REDAK; red++)
    {
        for (int stupac = 0; stupac < STUPAC; stupac++)
        {
            float x = stupac * velicina;
            float y = red * velicina + offset_y;

            celija.setPosition(x, y);
            celija.setFillColor(sf::Color(195, 255, 90));
            window.draw(celija);

            if (mapa[red][stupac] == ZID)
            {
                celija.setFillColor(sf::Color(24, 51, 0));
                window.draw(celija);
            }

            if (mapa[red][stupac] == VOCE)
            {
                celija.setFillColor(sf::Color(233, 51, 51));
                window.draw(celija);
            }

            if (mapa[red][stupac] == B)
            {
                celija.setFillColor(sf::Color(112, 24, 24));
                window.draw(celija);
            }
        }
    }

    for (int i = 0; i < zmija.size(); i++)
    {
        celija.setPosition(
            zmija[i].y * velicina,
            zmija[i].x * velicina + offset_y
        );

        if (i == 0)
        {
            celija.setFillColor(sf::Color(0, 127, 0));
        }
        else if (i % 2 == 0)
        {
            celija.setFillColor(sf::Color(0, 127, 0));
        }
        else
        {
            celija.setFillColor(sf::Color(70, 204, 97));
        }

        window.draw(celija);
    }
}