#include "utils/assert.h"
#include "map_parser.h"

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

	assert(data.get(0, 0) == '1');
	assert(data.get(0, 1) == '2');
	assert(data.get(0, 2) == '3');
	assert(data.get(0, 3) == '4');
	assert(data.get(0, 4) == '5');

	assert(data.get(1, 0) == 'a');
	assert(data.get(1, 1) == 's');
	assert(data.get(1, 2) == 'd');
	assert(data.get(1, 3) == 'w');
	assert(data.get(1, 4) == 'g');

	assert(data.get(5, 5) == 0);

	assert(data.get_score_by_pos(5) == 0);
	assert(data.get_score_by_pos(6) == 1);
	assert(data.get_score_by_pos(7) == 2);
	assert(data.get_score_by_pos(8) == 3);

	return 0;
}
