#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "global.h"

class Map
{
public:
	Map();
	void draw(sf::RenderWindow& window);
	void change_tile(const sf::Vector2f &position, const Cell_Type& type);
	Cell_Type get_cell_type(const sf::Vector2f& position) const;
	void pathfinding(sf::RenderWindow& window);
	void reset_cells();
	void clear();
	void set_diagonal_movement();
private:

private:
	std::array<std::array<Cell, MAP_WIDTH>, MAP_HEIGHT> map;
	sf::RectangleShape start;
	sf::RectangleShape end;

	std::vector<sf::Vector2f> target_position;

	bool diagonal_movement;
};

