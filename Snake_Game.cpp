#include <SFML/Graphics.hpp>
#include "Snake_Game.h"
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <ctime>

//provjera izlaska iz mape
bool Snake_Game::izvan(int red, int stupac)
{
    return (red < 0 || red >= REDAK || stupac < 0 || stupac >= STUPAC);
}

Snake_Game::Snake_Game()
{
    srand((unsigned)time(0));

    for (int red = 0; red < REDAK; red++)
        for (int stupac = 0; stupac < STUPAC; stupac++)
            mapa[red][stupac] = PRAZNO;

    faza = 0;

    raste = false;

    generirajZmiju();
    generirajZidove();
    generirajA_i_B();
    generirajVoce();

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
    for (int i = 0; i < REDAK; i++)
    {
        for (int j = 0; j < STUPAC; j++)
        {
            if (mapa[i][j] != A && mapa[i][j] != B)
                mapa[i][j] = PRAZNO;
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

//A start, B cilj
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

// 2 vocke samo na pocetku
void Snake_Game::generirajVoce()
{
    voce.clear();

    while (voce.size() < 2)
    {
        int red = rand() % REDAK;
        int stupac = rand() % STUPAC;

        if (mapa[red][stupac] != PRAZNO)
            continue;

        voce.push_back({ red, stupac });
        mapa[red][stupac] = VOCE;
    }
}

void Snake_Game::izracunajPut()
{
    struct Node
    {
        int red;
        int stupac;
    };

    bool posjeceno_polje[REDAK][STUPAC] = { false };
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

    int red_cilj, stupac_cilj;
    odrediCilj(red_cilj, stupac_cilj);

    udaljenost[red_pocetak][stupac_pocetak] = 0;
    red_cekanja.push({ red_pocetak, stupac_pocetak });

    int pomak_red[4] = { -1, 1, 0, 0 };
    int pomak_stupac[4] = { 0, 0, -1, 1 };

    while (!red_cekanja.empty())
    {
        Node trenutni_node = red_cekanja.front();
        red_cekanja.pop();

        if (posjeceno_polje[trenutni_node.red][trenutni_node.stupac])
            continue;

        posjeceno_polje[trenutni_node.red][trenutni_node.stupac] = true;

        for (int i = 0; i < 4; i++)
        {
            int novi_red = trenutni_node.red + pomak_red[i];
            int novi_stupac = trenutni_node.stupac + pomak_stupac[i];

            if (izvan(novi_red, novi_stupac))
                continue;

            if (mapa[novi_red][novi_stupac] == ZID)
                continue;

            if (udaljenost[novi_red][novi_stupac] >
                udaljenost[trenutni_node.red][trenutni_node.stupac] + 1)
            {
                udaljenost[novi_red][novi_stupac] =
                    udaljenost[trenutni_node.red][trenutni_node.stupac] + 1;

                prethodni[novi_red][novi_stupac] =
                    sf::Vector2i(trenutni_node.red, trenutni_node.stupac);

                red_cekanja.push({ novi_red, novi_stupac });
            }
        }
    }

    put.clear();

    if (udaljenost[red_cilj][stupac_cilj] == max_broj_koraka)
        return;

    sf::Vector2i trenutni(red_cilj, stupac_cilj);

    while (!(trenutni.x == red_pocetak && trenutni.y == stupac_pocetak))
    {
        put.push_back(trenutni);

        sf::Vector2i sljedeci = prethodni[trenutni.x][trenutni.y];

        if (sljedeci.x == -1)
            break;

        trenutni = sljedeci;
    }

    std::reverse(put.begin(), put.end());
}

void Snake_Game::odrediCilj(int& red, int& stupac)
{
    if (faza == 0)
    {
        red = voce[0].x;
        stupac = voce[0].y;
    }
    else if (faza == 1)
    {
        red = voce[1].x;
        stupac = voce[1].y;
    }
    else
    {
        red = b_red;
        stupac = b_stupac;
    }
}

void Snake_Game::update()
{
    if (put.empty())
    {
        izracunajPut();
        return;
    }

    sf::Vector2i nova_glava = put.front();
    put.erase(put.begin());

    zmija.insert(zmija.begin(), nova_glava);

    //zmija ne raste i rep se brise
    if (!raste)
    {
        sf::Vector2i stari_rep = zmija.back();
        zmija.pop_back();

        if (mapa[stari_rep.x][stari_rep.y] != ZID)
            mapa[stari_rep.x][stari_rep.y] = PRAZNO;
    }
    else
    {
        //zmija raste pa se rep ostavlja
        raste = false;
    }

    if (mapa[nova_glava.x][nova_glava.y] == VOCE)
    {
        if (faza == 0)
            prvo_voce_index = 0;
        else if (faza == 1)
            prvo_voce_index = 1;

        faza++;

        raste = true; //zmija raste za 1 blok

        mapa[nova_glava.x][nova_glava.y] = PRAZNO;

        put.clear();
        izracunajPut();
        return;
    }

    if (mapa[nova_glava.x][nova_glava.y] == B)
    {
    }
}

void Snake_Game::iscrtaj(sf::RenderWindow& window)
{
    int velicina = 20;

    sf::RectangleShape celija(sf::Vector2f(20.f, 20.f));

    for (int red = 0; red < REDAK; red++)
    {
        for (int stupac = 0; stupac < STUPAC; stupac++)
        {
            celija.setFillColor(sf::Color(195, 255, 90));
            celija.setPosition(stupac * velicina, red * velicina);
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

    // ZMIJA (zadnji sloj da prekriva sve)
    for (int i = 0; i < zmija.size(); i++)
    {
        if (i == 0)
        {
            celija.setFillColor(sf::Color(0, 127, 0)); // glava
        }
        else
        {
            if (i % 2 == 1)
                celija.setFillColor(sf::Color(70, 204, 97)); // tamnp zelena
            else
                celija.setFillColor(sf::Color(0, 127, 0)); // svijetlo zelena
        }

        celija.setPosition(zmija[i].y * velicina, zmija[i].x * velicina);
        window.draw(celija);
    }
}