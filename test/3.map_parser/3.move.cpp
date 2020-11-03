#include "modules/map_parser.h"
#include "utils/assert.h"

int main()
{
	char map[] =
		"12345"
		"asdwg"
		"99999"
		"54321"
		"12345";
	std::vector<int> *positions = new std::vector<int>();
	positions->push_back(5);
	positions->push_back(6);
	positions->push_back(7);
	positions->push_back(8);

	std::vector<int> *scores = new std::vector<int>();
	scores->push_back(0);
	scores->push_back(1);
	scores->push_back(2);
	scores->push_back(3);

	class server_data data(map, 5, 0, *positions, *scores);

	delete positions;
	delete scores;

	assert(data.clone().move(move_op_left) == 1);

	{
		class server_data cloned = data.clone();
		assert(cloned.move(move_op_down) == 0);
		assert(cloned.get(1, 0) == 's');
		assert(cloned.move(move_op_down) == 1);
		assert(cloned.get_score_by_pos(5) == 0);
	}

	{
		class server_data cloned = data.clone();
		assert(cloned.move(move_op_right) == 0);
		assert(cloned.get(1, 0) == 'd');
		assert(cloned.move(move_op_right) == 0);
		assert(cloned.get(1, 0) == '0');
		assert(cloned.get(1, 1) == '0');
		assert(cloned.get_score_by_pos(5) == 0);
	}

	{
		class server_data cloned = data.clone();
		assert(cloned.move(move_op_up) == 0);
		assert(cloned.get(1, 0) == 'w');
		assert(cloned.move(move_op_up) == 0);
		assert(cloned.get(1, 0) == '0');
		assert(cloned.get(0, 0) == 'w');
		assert(cloned.get_score_by_pos(5) == 1);
	}

	return 0;
}
