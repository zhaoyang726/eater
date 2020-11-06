#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <error.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <thread>
#include "modules/network.h"
#include <string>
static int read_signal = 0;
static int heart_signal = 0;
static char ok[20] = {0};
static char start[20] = {0};
static char game[20] = {0};
static std::thread *read_t = NULL;
static std::thread *heart_t = NULL;
static int send_code = 1, send_reday = 2;
int send_message(int flag);


int connect(uint32_t ip_addr, uint32_t port)

{
    if (!ip_addr) {
        printf("ip_addr is null!\n");
        return  1;
    }

    if (!port) {
        printf("port is null!\n");
        return  1;
    }

    int st = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    char ip_address[20];
    sprintf(ip_address, "%d.%d.%d.%d",
            (ip_addr & 0xff000000) >> 24,
            (ip_addr & 0x00ff0000) >> 16,
            (ip_addr & 0x0000ff00) >> 8,
            (ip_addr & 0x000000ff) >> 0);

    if (inet_pton(AF_INET, ip_address, &addr.sin_addr) <= 0) {
        printf("inet_pton error for %s\n", ip_address);
        return 0;
    }

    if (connect(st, (struct sockaddr *) & addr, sizeof(addr)) == -1) {
        printf("connect fail %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    if (!send_message(send_code)) {
        if (start_heartbeat_thread()) {
            printf("error to start heartbeat_thread!");
            return 1;
        }
    } else {
        printf("send code faild!");
        return 1;
    }

    if (!send_message(send_reday)) {
        if (start_read_thread()) {
            printf("error to start heartbeat_thread!");
            return 1;
        }
    } else {
        printf("send ready faild!");
        return 1;
    }

    return 0;
}

int send_message(int flag) {
    char key[] = "c96f4d7661c94cbb9706469649a7cbbc";
    char ready[] = "(READY)";
    int st = socket(AF_INET, SOCK_STREAM, 0);
    int sc = send(st, key, sizeof(key), 0);
    int sockfd;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 1;
    }

    switch (flag) {
        case 1:
            if (send(sockfd, key, sizeof(key), 0) < 0) {
                printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
                return 1;
            }

            break;

        case 2:

            //if (start == "1" || game == "3") {
            if (send(sockfd, ready, sizeof(ready), 0) < 0) {
                printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
                return 1;
            }

            //}
            break;

        default:
            printf("send_message has done!");
            break;
    }

    heart_signal = 0;
    return 0;
}
int _read() {//
    int sockfd;
    char s[1024];

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 1;
    }

    while (!read_signal) {
        memset(s, 0, sizeof(s));
        int rc = recv(sockfd, s, sizeof(s), 0);

        if (rc <= 0) { //代表socket被关闭（0）或者出错（-1）
            printf("socket disconnect!");
            break;
        } else if (sizeof(s) == 12) {
            strncpy(start, "1", 1);
        } else if (s == "[OK]") {
            strncpy(ok, "2", 1);
        } else if (s == "[GAMEOVER]") {
            strncpy(start, "3", 1);
        } else {
            get_server_data(s);//不是前面的数据则是地图数据，传给get_server_data
        }

        printf("client receive:%s\n", s);
        sleep(1);
    }

    return 0;
}
int heartbeat() {
    int sockfd;
    char beat[] = "(H)";

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 1;
    }

    while (!heart_signal) {
        if (send(sockfd, beat, sizeof(beat), 0) < 0) {
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
            return 1;
        }

        sleep(1);
    }

    heart_signal = 0;
    return 0;
}
int start_read_thread() {
    read_t = new std::thread(_read);
    return 0;
}

int finish_read_thread() {
    heart_signal = 1;
    read_t->join();
    std::thread *read_t = NULL;
    delete read_t;
    return 0;
}

int start_heartbeat_thread() {
    heart_t = new std::thread(heartbeat);
    return 0;
}

int finish_heartbeat_thread() {
    heart_signal = 1;
    heart_t->join();
    std::thread *heart_t = NULL;
    delete heart_t;
    return 0;
}

int disconnect() {
    return 0;
}
int get_server_data(char *buf) {
    printf("client receive:%s\n", buf);
    return 0;
}

int send_operating(enum move_operating move_op, bool is_fire) {
    char s[1024];
    int sockfd;
    memset(s, '\0', sizeof(s));

    if (move_op == move_op_stay && is_fire != 0) {//不动，发炮？
        strncpy(s, "A@v", 3);
    }

    if (move_op == 	move_op_up && is_fire != 0) {
        strncpy(s, "A@wv", 4);
    }

    if (move_op == move_op_up && is_fire == 0) {
        strncpy(s, "A@w ", 4);
    }

    if (move_op == move_op_down && is_fire != 0) {
        strncpy(s, "A@sv", 4);
    }

    if (move_op == move_op_down && is_fire == 0) {
        strncpy(s, "A@s ", 4);
    }

    if (move_op == move_op_left && is_fire != 0) {
        strncpy(s, "A@av", 4);
    }

    if (move_op == move_op_left && is_fire == 0) {
        strncpy(s, "A@a ", 4);
    }

    if (move_op == move_op_right && is_fire != 0) {
        strncpy(s, "A@dv", 4);
    }

    if (move_op == move_op_right && is_fire == 0) {
        strncpy(s, "A@d ", 4);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 1;
    }

    int sc = send(sockfd, s, sizeof(s), 0);
    return 0;
}
