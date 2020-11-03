#include "modules/algorithm.h"
#include "utils/assert.h"

int main()
{
	char map[] =
		"12345"
		"0sdwg"
		"99999"
		"54321"
		"w2345";
	std::vector<int> positions;
	positions.push_back(20);
	positions.push_back(6);
	positions.push_back(7);
	positions.push_back(8);

	std::vector<int> scores;
	scores.push_back(10);
	scores.push_back(1);
	scores.push_back(2);
	scores.push_back(3);

	class server_data data(map, 5, 0, positions, scores);

	auto result = algorithm(data);
	assert(result.first == move_op_up);
	assert(result.second == false);

	return 0;
}
