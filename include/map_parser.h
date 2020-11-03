#include <stdint.h>
#include <vector>
#include "network.h"

class server_data {
	public:
		// 构造函数
		server_data(char *map, int size, int id, std::vector<int> &poses, std::vector<int> &scores);
		// 临时空构造函数
		server_data();

		// 析构函数
		~server_data();

		// 克隆一个数据
		class server_data clone();

		// 获得地图的大小
		int get_map_size();

		// 获得地图上某点的字符
		// 越界返回0
		char get(int row, int col);

		// 获得pos位置的玩家分数
		int get_score_by_pos(int pos);

		/* 移动
		 * 返回值:
		 * - 0: 成功
		 * - 1: 失败
		 * */
		int move(enum move_operating move_op);

		/* 开炮
		 * 返回值:
		 * - 0: 成功
		 * - 1: 失败
		 * */
		int fire();
	private:
		// 位置数组
		std::vector<int> _positions;

		// 得分数组
		std::vector<int> _scores;

		// 己方编号
		int _id;

		// 地图大小
		int _size;

		// 地图字符指针
		char *_map;
};
