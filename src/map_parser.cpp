#include "map_parser.h"

server_data::server_data()
{
}

server_data::server_data(char *map,
		int size, int id, std::vector<int> &poses, std::vector<int> &scores)
{
	// stub
}

server_data::~server_data()
{
	// stub
}

class server_data server_data::clone()
{
	// stub
	return server_data();
}

int server_data::get_map_size()
{
	// stub
	return 0;
}

char server_data::get(int row, int col)
{
	// stub
	return 0;
}

int server_data::get_score_by_pos(int pos)
{
	// stub
	return 0;
}

int server_data::move(enum move_operating move_op)
{
	// stub
	return 0;
}

int server_data::fire()
{
	// stub
	return 0;
}
