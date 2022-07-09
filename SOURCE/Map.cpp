#include "Map.h"

Map::Map()
{
	for (size_t i = 0; i < MAP_HEIGHT; i++)
	{
		for (size_t j = 0; j < MAP_WIDTH; j++)
		{
			map[i][j].type = Cell_Type::FLOOR;
			map[i][j].row_index = i;
			map[i][j].column_index = j;
			map[i][j].rectangle.setSize(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
			map[i][j].rectangle.setPosition(sf::Vector2f(j * CELL_SIZE, i * CELL_SIZE));
			map[i][j].rectangle.setFillColor(sf::Color::White);
		}
	}

	start.setSize(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
	end.setSize(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

	start.setPosition(sf::Vector2f(2 * CELL_SIZE, 6 * CELL_SIZE));
	map[6][2].type = Cell_Type::START;
	end.setPosition(sf::Vector2f(2 * CELL_SIZE, 1 * CELL_SIZE));
	map[1][2].type = Cell_Type::END;

	start.setFillColor(sf::Color::Green);
	end.setFillColor(sf::Color::Red);

	diagonal_movement = false;		//disable or unable diagonal movement
}

void Map::set_diagonal_movement() 
{
	diagonal_movement = (!diagonal_movement) ? true : false;
}

Cell_Type Map::get_cell_type(const sf::Vector2f& position) const
{
	return map[std::floor(position.y / (CELL_SIZE))][std::floor(position.x / (CELL_SIZE))].type;
}

void Map::change_tile(const sf::Vector2f &position, const Cell_Type &type) 
{
	if ((position.x < 0 || position.x >= SCREEN_WIDTH) || (position.y < 0 || position.y >= SCREEN_HEIGHT))
	{
		return;
	}

	switch (type)
	{
	case Cell_Type::FLOOR:
		if (map[std::floor(position.y / (CELL_SIZE))][std::floor(position.x / (CELL_SIZE))].type == Cell_Type::START
			|| map[std::floor(position.y / (CELL_SIZE))][std::floor(position.x / (CELL_SIZE))].type == Cell_Type::END)
		{
			return;
		}
		map[std::floor(position.y / (CELL_SIZE))][std::floor(position.x / (CELL_SIZE))].rectangle.setFillColor(sf::Color(100, 100, 100));
		map[std::floor(position.y / (CELL_SIZE))][std::floor(position.x / (CELL_SIZE))].type = Cell_Type::WALL;
		break;
	case Cell_Type::WALL:
		if (map[std::floor(position.y / (CELL_SIZE))][std::floor(position.x / (CELL_SIZE))].type == Cell_Type::START 
			|| map[std::floor(position.y / (CELL_SIZE))][std::floor(position.x / (CELL_SIZE))].type == Cell_Type::END)
		{
			return;
		}
		map[std::floor(position.y / (CELL_SIZE))][std::floor(position.x / (CELL_SIZE))].rectangle.setFillColor(sf::Color::White);
		map[std::floor(position.y / (CELL_SIZE))][std::floor(position.x / (CELL_SIZE))].type = Cell_Type::FLOOR;
		break;
	case Cell_Type::START:
		if (map[std::floor(position.y / (CELL_SIZE))][std::floor(position.x / (CELL_SIZE))].type == Cell_Type::WALL)
		{
			return;
		}
		map[std::floor(start.getPosition().y / (CELL_SIZE))][std::floor(start.getPosition().x / (CELL_SIZE))].type = Cell_Type::FLOOR;
		start.setPosition(sf::Vector2f(std::floor(position.x / (CELL_SIZE)) * CELL_SIZE, std::floor(position.y / (CELL_SIZE)) * CELL_SIZE));
		map[std::floor(position.y / (CELL_SIZE))][std::floor(position.x / (CELL_SIZE))].type = Cell_Type::START;
		break;
	case Cell_Type::END:
		if (map[std::floor(position.y / (CELL_SIZE))][std::floor(position.x / (CELL_SIZE))].type == Cell_Type::WALL)
		{
			return;
		}
		map[std::floor(end.getPosition().y / (CELL_SIZE))][std::floor(end.getPosition().x / (CELL_SIZE))].type = Cell_Type::FLOOR;
		end.setPosition(sf::Vector2f(std::floor(position.x / (CELL_SIZE)) * CELL_SIZE, std::floor(position.y / (CELL_SIZE)) * CELL_SIZE));
		map[std::floor(position.y / (CELL_SIZE))][std::floor(position.x / (CELL_SIZE))].type = Cell_Type::END;
		break;
	default:
		break;
	}
}

void Map::draw(sf::RenderWindow& window) 
{
	for (size_t i = 0; i < MAP_HEIGHT; i++)
	{
		for (size_t j = 0; j < MAP_WIDTH; j++)
		{
			window.draw(map[i][j].rectangle);
		}
	}

	for (size_t i = 0; i < target_position.size(); i++)
	{
		if (i == target_position.size() - 1)
		{
			break;
		}

		sf::Vertex direction[] = { sf::Vertex(sf::Vector2f(target_position[i].x, target_position[i].y), sf::Color::Red),
			sf::Vertex(sf::Vector2f(target_position[i + 1].x, target_position[i + 1].y), sf::Color::Red) };
		window.draw(direction, 2, sf::Lines);
	}

	window.draw(start);
	window.draw(end);
}

void Map::clear() 
{
	for (size_t i = 0; i < MAP_HEIGHT; i++)
	{
		for (size_t j = 0; j < MAP_WIDTH; j++)
		{
			map[i][j].parent = nullptr;
			map[i][j].H = 0;
			map[i][j].G = 0;
			map[i][j].F = 0;
			map[i][j].rectangle.setFillColor(sf::Color::White);
			if (map[i][j].type != Cell_Type::START && map[i][j].type != Cell_Type::END)
			{
				map[i][j].type = Cell_Type::FLOOR;
			}
		}
	}

	target_position.clear();
}

void Map::reset_cells()
{
	for (size_t i = 0; i < MAP_HEIGHT; i++)
	{
		for (size_t j = 0; j < MAP_WIDTH; j++)
		{
			map[i][j].parent = nullptr;
			map[i][j].H = 0;
			map[i][j].G = 0;
			map[i][j].F = 0;
			if (map[i][j].type != Cell_Type::WALL)
			{
				map[i][j].rectangle.setFillColor(sf::Color::White);
			}
		}
	}
}

void Map::pathfinding(sf::RenderWindow& window)
{
	reset_cells();

	std::vector<Cell*> open_list;
	std::vector<Cell*> closed_list;
	std::vector<Cell*> path_list;
	std::vector<Cell*>::iterator itr;
	Cell* current_node = nullptr;

	Cell* start_node = &map[std::round(start.getPosition().y / CELL_SIZE)][std::round(start.getPosition().x / CELL_SIZE)];
	Cell* end_node = &map[std::round(end.getPosition().y / CELL_SIZE)][std::round(end.getPosition().x / CELL_SIZE)];

	if (start_node == end_node)
	{
		target_position.clear();

		return;
	}

	for (short i = 0; i < MAP_HEIGHT; i++)			//calculting H value for each node 
	{
		for (short j = 0; j < MAP_WIDTH; j++)
		{
			map[i][j].H = (fabs(end_node->row_index - i) + fabs(end_node->column_index - j)) * 10;
		}
	}

	open_list.push_back(start_node);

	int lowest_F, lowest_H;
	std::vector<Cell*> neighbour_nodes;
	Cell* node = nullptr;
	while (!open_list.empty())
	{
		lowest_F = INT_MAX;
		lowest_H = INT_MAX;
		for (auto node : open_list)
		{
			if (node->F < lowest_F)
			{
				lowest_F = node->F;
				lowest_H = node->H;
				current_node = node;
			}
			else if (node->F == lowest_F)
			{
				if (node->H < lowest_H)
				{
					lowest_F = node->F;
					lowest_H = node->H;
					current_node = node;
				}
			}
		}

		itr = std::find(open_list.begin(), open_list.end(), current_node);
		if (itr != open_list.end())
		{
			open_list.erase(itr);
		}

		current_node->rectangle.setFillColor(sf::Color::Cyan);
		closed_list.push_back(current_node);

		//adding all nodes to the list that are neighbour to our current node
		for (short i = -1; i < 2; i++)
		{
			for (short j = -1; j < 2; j++)		
			{
				if (i != 0 || j != 0)
				{
					if ((j + i == 0 || fabs(j + i) == 2))	//check if it is a diagonal and whether diagonal movement is allowed
					{
						if (!diagonal_movement)
						{
							continue;
						}

						if (map[current_node->row_index + i][current_node->column_index].type == Cell_Type::WALL
							&& map[current_node->row_index][current_node->column_index + j].type == Cell_Type::WALL)
						{
							continue;
						}
					}

					if ((current_node->row_index + i >= 0 && current_node->row_index + i < MAP_HEIGHT)			//check node's boundaries
						&& (current_node->column_index + j >= 0 && current_node->column_index + j < MAP_WIDTH))
					{
						itr = std::find(closed_list.begin(), closed_list.end(), &map[current_node->row_index + i][current_node->column_index + j]);
						if ((map[current_node->row_index + i][current_node->column_index + j].type == Cell_Type::FLOOR ||
							map[current_node->row_index + i][current_node->column_index + j].type == Cell_Type::END)
							&& itr == closed_list.end())
						{
							neighbour_nodes.push_back(&map[current_node->row_index + i][current_node->column_index + j]);

							node = &map[current_node->row_index + i][current_node->column_index + j];

							if (node == end_node)
							{
								node->rectangle.setFillColor(sf::Color::Yellow);
								node->parent = current_node;

								while (node != nullptr)
								{
									path_list.push_back(node);
									node = node->parent;
								}

								open_list.clear();
								i = 2;
								break;
							}

							itr = std::find(open_list.begin(), open_list.end(), &map[current_node->row_index + i][current_node->column_index + j]);
							if (itr == open_list.end())
							{
								node->rectangle.setFillColor(sf::Color::Yellow);
								open_list.push_back(node);

								node->parent = current_node;

								int value = (j + i == 0 || fabs(j + i) == 2) ? 14 : 10;
								node->G = current_node->G + value;
								node->F = node->G + node->H;
							}
							else
							{
								int value = (j + i == 0 || fabs(j + i) == 2) ? 14 : 10;
								int G = current_node->G + value;
								if (G < node->G)
								{
									node->parent = current_node;
									node->G = G;
								}
								node->F = node->G + node->H;
							}
						}
					}
				}
			}
		}
	}

	target_position.clear();
	for (auto node : path_list)
	{
		map[node->row_index][node->column_index].rectangle.setFillColor(sf::Color::Cyan);
		target_position.push_back(sf::Vector2f(node->column_index * CELL_SIZE + CELL_SIZE / 2, node->row_index * CELL_SIZE + CELL_SIZE / 2));
	}

	if (!target_position.size())
	{
		return;
	}
}
