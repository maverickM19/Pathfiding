#include <SFML/Graphics.hpp>
#include "Map.h"
#include "global.h"

int main()
{  
    sf::RenderWindow window(sf::VideoMode(1280, 960), "Pathfinding");
    Map map;
    bool draw = false;
    Cell_Type type;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Key::Enter)
                {
                   map.pathfinding(window);
                }
                if (event.key.code == sf::Keyboard::Key::R)
                {
                    map.clear();
                }
                if (event.key.code == sf::Keyboard::Key::D)
                {
                    map.set_diagonal_movement();
                }
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.key.code == sf::Mouse::Left)
                {
                    draw = true;
                    type = map.get_cell_type(sf::Vector2f(sf::Mouse::getPosition(window)));
                }
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.key.code == sf::Mouse::Left)
                {
                    draw = false;
                }
            }
        }

        if (draw)
        {
            map.change_tile(sf::Vector2f(sf::Mouse::getPosition(window)), type);
        }

        window.clear();
        map.draw(window);
        window.display();
    }

    return 0;
}
