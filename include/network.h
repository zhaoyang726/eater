#include <stdint.h>

/* 连接服务器，发送key
 * 参数:
 * - ip_addr: ip地址，0xAABBCCDD为AA.BB.CC.DD
 * - port: 端口号
 * 返回值:
 * - 0: 成功
 * - 1: 失败
 * */
int connect(uint32_t ip_addr, uint32_t port);

/* 开启读线程，处理心跳包和数据包
 * 返回值:
 * - 0: 成功
 * - 1: 失败
 * */
int start_read_thread();

/* 关闭读线程
 * 返回值:
 * - 0: 成功
 * - 1: 失败
 * */
int finish_read_thread();

/* 与服务器断开连接
 * 返回值:
 * - 0: 成功
 * - 1: 失败
 * */
int disconnect();

/* 获得一次服务器数据
 * 可堵塞
 * 参数:
 * - buf: 字符串返回值，由调用者初始化
 * 返回值:
 * - 0: 成功
 * - 1: 失败
 * */
int get_server_data(char *buf);

// 移动操作
enum move_operating {
	move_op_stay,  // 不动
	move_op_up,    // 向上移动
	move_op_down,  // 向下移动
	move_op_left,  // 向左移动
	move_op_right, // 向右移动
};

/* 发送一次操作
 * 参数:
 * - move_op: 移动操作
 * - is_fire: 是否开炮
 * 返回值:
 * - 0: 成功
 * - 1: 失败
 * */
int send_operating(enum move_operating move_op, bool is_fire);
