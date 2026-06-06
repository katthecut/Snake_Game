#include <SFML/Graphics.hpp>
#include "Snake_Game.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 400), "Dijkstra Snake");
    window.setFramerateLimit(60);

    Snake_Game igra;

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (clock.getElapsedTime().asMilliseconds() > 200)
        {
            igra.update();
            clock.restart();
        }

        window.clear(sf::Color(123, 179, 105));

        igra.iscrtaj(window);

        window.display();
    }

    return 0;
}
