#pragma once

const unsigned short SCREEN_HEIGHT = 960;
const unsigned short SCREEN_WIDTH = 1280;

const unsigned short MAP_HEIGHT = 30;
const unsigned short MAP_WIDTH = 40;
const unsigned short CELL_SIZE = 32;

enum class Cell_Type
{
	FLOOR,
	WALL,
	START,
	END
};

struct Cell
{
	Cell_Type type;
	sf::RectangleShape rectangle;

	short column_index;
	short row_index;

	short H;
	short G; 
	short F; 

	Cell* parent;
};
