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
static std::thread *read_thread = NULL;
static std::thread *heart_thread = NULL;
static int send_code = 1, send_reday = 2;
static int fd = 0;
int send_message(int flag);

int connect(uint32_t ip_addr, uint32_t port)

{
    char key[] = "(c96f4d7661c94cbb9706469649a7cbbc)";
    char s[1024];

    if (!ip_addr) {
        printf("ip_addr is null!\n");
        return  1;
    }

    if (!port) {
        printf("port is null!\n");
        return  1;
    }

    fd = socket(AF_INET, SOCK_STREAM, 0);
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

    if (connect(fd, (struct sockaddr *) & addr, sizeof(addr)) == -1) {
        printf("connect fail %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    send(fd, key, sizeof(key), 0);
    int rc = recv(fd, s, sizeof(s), 0);

    if (!strcmp("[OK]", s)) {
        strncpy(ok, "[OK]", 4);
        printf("HHH\n");
    }

    /*if (!send_message(send_code)) {
        if (start_heartbeat_thread()) {
            printf("error to start heartbeat_thread!");
            return 1;
        } //write(Pipe[1],"true",5);
    } else {
        printf("send code faild!");
        return 1;
    }

    printf("%s: %d\n", __FILE__, __LINE__);

    if (!send_message(send_reday)) {
        if (start_read_threadhread()) {
            printf("error to start heartbeat_thread!");
            return 1;
        }
    } else {
        printf("send ready faild!");
        return 1;
    }
    */
    return 0;
}

int send_message(int flag) {
    char key[] = "(c96f4d7661c94cbb9706469649a7cbbc)";
    char ready[] = "(READY)";

    switch (flag) {
        case 1:
            if (send(fd, key, sizeof(key), 0) < 0) {
                printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
                return 1;
            }

            break;

        case 2:

            //if (start == "1" || game == "3") {
            if (send(fd, ready, sizeof(ready), 0) < 0) {
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
    char s[1024];
    char ready[] = "(READY)";
    char buffer[10];
    //send(fd, key, sizeof(key), 0);
    //send(fd, ready, sizeof(ready), 0);
    //recv(fd, s, sizeof(s), 0);

    //Flag=false;
    if (!strcmp("[OK]", ok)) {
        printf("%s: %d\n", __FILE__, __LINE__);
        memset(s, 0, sizeof(s));
        recv(fd, s, sizeof(s), 0);
        printf("%s: %d\n", __FILE__, __LINE__);
        printf("ok %s\n", s);

        if (!strcmp("[START 1 5]", s)) {
            send(fd, ready, sizeof(ready), 0);
            printf("start %s\n", s);
            memset(s, 0, sizeof(s));
            recv(fd, s, sizeof(s), 0);
            printf("start111 %s\n", s);
            printf("%s: %d\n", __FILE__, __LINE__);
        }
    }

    printf("read signal is %d\n", !read_signal);

    while (!read_signal) {
        memset(s, 0, sizeof(s));
        printf("%s: %d\n", __FILE__, __LINE__);
        int rc = recv(fd, s, sizeof(s), 0);
        printf("%s: %d\n", __FILE__, __LINE__);

        if (rc <= 0) { //代表socket被关闭（0）或者出错（-1）
            printf("socket disconnect!");
            break;
        }

        printf("%s: %d\n", __FILE__, __LINE__);
        printf("client receive:%s\n", s);
        /*else if (sizeof(s) == 12) {
            strncpy(start, "1", 1);
        } else if (s == "[OK]") {
            strncpy(ok, "2", 1);
        } else if (s == "[GAMEOVER]") {
            strncpy(start, "3", 1);
        }

        else {
            get_server_data(s);//不是前面的数据则是地图数据，传给get_server_data
        }
        */
    }

    read_signal = 1;
    return 0;
}
int heartbeat() {
    char beat[] = "(H)";
    char s[1024];
    printf("%s: %d\n", __FILE__, __LINE__);

    while (!heart_signal) {
        if (send(fd, beat, sizeof(beat), 0) < 0) {
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
            return 1;
        }

        recv(fd, s, sizeof(s), 0);
        printf("recive %s\n", s);
        sleep(1);
    }

    heart_signal = 0;
    return 0;
}
int start_read_thread() {
    read_thread = new std::thread(_read);
    return 0;
}

int finish_read_thread() {
    read_signal = 1;
    read_thread->join();
    delete read_thread;
    read_thread = NULL;
    return 0;
}

int start_heartbeat_thread() {
    heart_thread = new std::thread(heartbeat);
    return 0;
}

int finish_heartbeat_thread() {
    heart_signal = 1;
    heart_thread->join();
    delete heart_thread;
    heart_thread = NULL;
    return 0;
}

int disconnect() {
    close(fd);
    return 0;
}
int get_server_data(char *buf) {
    printf("get_server_data:%s\n", buf);
    return 0;
}

int send_operating(enum move_operating move_op, bool is_fire) {
    char s[1024];
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

    int sc = send(fd, s, sizeof(s), 0);
    return 0;
}
