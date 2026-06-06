#include <SFML/Graphics.hpp>
#include "Snake_Game.h"
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 400), "Snake");
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

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    igra.izvrsiPostavljanje({ event.mouseButton.x, event.mouseButton.y });
                }
            }
        }

        if (clock.getElapsedTime().asMilliseconds() > 200)
        {
            igra.update();
            clock.restart();
        }

        window.clear();
        igra.iscrtaj(window);
        window.display();
    }

    return 0;
}