#include "modules/algorithm.h"
#include "utils/assert.h"

int main()
{
	char map1[] =
		"12345"
		"asdwg"
		"99999"
		"54321"
		"12345";
	char map2[] =
		"12345"
		"0sdwg"
		"99999"
		"54321"
		"a2345";
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

	class server_data data1(map1, 5, 0, positions, scores);
	class server_data data2(map2, 5, 0, positions, scores);

	int score1;
	int score2;
	assert(evaluate(data1, score1) == 0);
	assert(evaluate(data2, score2) == 0);
	assert(score1 < score2);

	return 0;
}
