#include "map_parser.h"

server_data::server_data()
{
}

server_data::server_data(char *map,
		int size, int id, std::vector<int> &poses, std::vector<int> &scores)
{
	// TODO
}

server_data::~server_data()
{
	// TODO
}

class server_data server_data::clone()
{
	// TODO
	return server_data();
}

int server_data::get_map_size()
{
	// TODO
	return 0;
}

char server_data::get(int row, int col)
{
	// TODO
	return 0;
}

int server_data::get_score_by_pos(int pos)
{
	// TODO
	return 0;
}

int server_data::move(enum move_operating move_op)
{
	// TODO
	return 0;
}

int server_data::fire()
{
	// TODO
	return 0;
}
