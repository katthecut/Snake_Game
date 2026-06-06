#include "Snake_Game.h"
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <ctime>

bool Snake_Game::izvan(int redak, int stupac)
{
    return (redak < 0 || redak >= REDAK || stupac < 0 || stupac >= STUPAC);
}

Snake_Game::Snake_Game()
{
    srand((unsigned)time(0));

    for (int i = 0; i < REDAK; i++)
        for (int j = 0; j < STUPAC; j++)
            mapa[i][j] = PRAZNO;

    a_redak = 0;
    a_stupac = 0;

    b_redak = 10;
    b_stupac = 20;

    mapa[a_redak][a_stupac] = A;
    mapa[b_redak][b_stupac] = B;

    generirajZidove();

    zmija.clear();

    zmija.push_back(sf::Vector2i(0, 2));
    zmija.push_back(sf::Vector2i(0, 1));
    zmija.push_back(sf::Vector2i(0, 0));

    generirajVoce();

    idi_na_voce = true;

    izracunajPut();
}

void Snake_Game::generirajVoce()
{
    voce.clear();

    while (voce.size() < 2)
    {
        int redak = rand() % REDAK;
        int stupac = rand() % STUPAC;

        if (mapa[redak][stupac] == ZID)
            continue;

        if (redak == a_redak && stupac == a_stupac)
            continue;

        if (redak == b_redak && stupac == b_stupac)
            continue;

        bool postoji = false;

        for (auto& v : voce)
        {
            if (v.x == redak && v.y == stupac)
            {
                postoji = true;
                break;
            }
        }

        if (postoji)
            continue;

        voce.push_back(sf::Vector2i(redak, stupac));
        mapa[redak][stupac] = VOCE;
    }
}

void Snake_Game::odrediCilj(int& cilj_redak, int& cilj_stupac)
{
    if (idi_na_voce && !voce.empty())
    {
        cilj_redak = voce[0].x;
        cilj_stupac = voce[0].y;
    }
    else
    {
        cilj_redak = b_redak;
        cilj_stupac = b_stupac;
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

void Snake_Game::izracunajPut()
{
    struct Node
    {
        int redak;
        int stupac;
        int udaljenost;

        bool operator>(const Node& o) const
        {
            return udaljenost > o.udaljenost;
        }
    };

    const int BESKONACNO = 1000000;

    int distance[REDAK][STUPAC];
    sf::Vector2i prethodni[REDAK][STUPAC];
    bool visited[REDAK][STUPAC] = { false };

    for (int redak = 0; redak < REDAK; redak++)
        for (int stupac = 0; stupac < STUPAC; stupac++)
        {
            distance[redak][stupac] = BESKONACNO;
            prethodni[redak][stupac] = sf::Vector2i(-1, -1);
        }

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

    int cilj_redak, cilj_stupac;
    odrediCilj(cilj_redak, cilj_stupac);

    int start_redak = zmija.front().x;
    int start_stupac = zmija.front().y;

    distance[start_redak][start_stupac] = 0;
    pq.push({ start_redak, start_stupac, 0 });

    int pomak_redak[4] = { -1, 1, 0, 0 };
    int pomak_stupac[4] = { 0, 0, -1, 1 };

    while (!pq.empty())
    {
        Node trenutni = pq.top();
        pq.pop();

        if (visited[trenutni.redak][trenutni.stupac])
            continue;

        visited[trenutni.redak][trenutni.stupac] = true;

        for (int i = 0; i < 4; i++)
        {
            int novi_redak = trenutni.redak + pomak_redak[i];
            int novi_stupac = trenutni.stupac + pomak_stupac[i];

            if (izvan(novi_redak, novi_stupac))
                continue;

            if (mapa[novi_redak][novi_stupac] == ZID)
                continue;

            if (distance[novi_redak][novi_stupac] >
                distance[trenutni.redak][trenutni.stupac] + 1)
            {
                distance[novi_redak][novi_stupac] =
                    distance[trenutni.redak][trenutni.stupac] + 1;

                prethodni[novi_redak][novi_stupac] =
                    sf::Vector2i(trenutni.redak, trenutni.stupac);

                pq.push({ novi_redak, novi_stupac, distance[novi_redak][novi_stupac] });
            }
        }
    }

    if (distance[cilj_redak][cilj_stupac] == BESKONACNO)
    {
        put.clear();
        return;
    }

    put.clear();

    sf::Vector2i trenutni(cilj_redak, cilj_stupac);

    if (prethodni[cilj_redak][cilj_stupac].x == -1)
        return;

    while (trenutni.x != -1)
    {
        put.push_back(trenutni);
        trenutni = prethodni[trenutni.x][trenutni.y];
    }

    std::reverse(put.begin(), put.end());
}

void Snake_Game::update()
{
    if (put.empty())
    {
        izracunajPut();
        return;
    }

    sf::Vector2i stara_repna_pozicija = zmija.back();
    mapa[stara_repna_pozicija.x][stara_repna_pozicija.y] = PRAZNO;

    sf::Vector2i nova_glava = put.front();
    put.erase(put.begin());

    zmija.insert(zmija.begin(), nova_glava);
    zmija.pop_back();

    if (mapa[nova_glava.x][nova_glava.y] == VOCE)
    {
        idi_na_voce = true;
        generirajVoce();
    }

    for (int redak = 0; redak < REDAK; redak++)
        for (int stupac = 0; stupac < STUPAC; stupac++)
        {
            if (mapa[redak][stupac] == ZID) continue;
            if (mapa[redak][stupac] == VOCE) continue;
            if (mapa[redak][stupac] == A || mapa[redak][stupac] == B) continue;
            mapa[redak][stupac] = PRAZNO;
        }

    for (auto& dio : zmija)
        mapa[dio.x][dio.y] = A;
}

void Snake_Game::iscrtaj(sf::RenderWindow& window)
{
    int velicina = 20;

    sf::RectangleShape celija(sf::Vector2f(20.f, 20.f));
    celija.setOutlineThickness(0.f);

    for (int redak = 0; redak < REDAK; redak++)
        for (int stupac = 0; stupac < STUPAC; stupac++)
        {
            sf::Color boja;

            if (mapa[redak][stupac] == PRAZNO)
                continue;

            if (mapa[redak][stupac] == ZID)
                boja = sf::Color(80, 94, 82);

            else if (redak == a_redak && stupac == a_stupac)
                boja = sf::Color(0, 255, 0);

            else if (redak == b_redak && stupac == b_stupac)
                boja = sf::Color(173, 24, 35);

            else if (mapa[redak][stupac] == VOCE)
                boja = sf::Color(160, 60, 200);

            else
                continue;

            celija.setFillColor(boja);
            celija.setPosition(stupac * velicina, redak * velicina);
            window.draw(celija);
        }

    for (int i = 0; i < zmija.size(); i++)
    {
        celija.setFillColor(i % 2 == 0 ?
            sf::Color(120, 255, 120) :
            sf::Color(40, 180, 40));

        celija.setPosition(
            zmija[i].y * velicina,
            zmija[i].x * velicina
        );

        window.draw(celija);
    }
}