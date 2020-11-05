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
static int read_signal = 0;
static int heart_signal = 0;

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
    addr.sin_addr.s_addr = htonl(ip_addr);

    if (connect(st, (struct sockaddr *) & addr, sizeof(addr)) == -1) {
        printf("connect fail %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    return 0;
}

int sendsocket() {
    char key[] = "c96f4d7661c94cbb9706469649a7cbbc";
    int st = socket(AF_INET, SOCK_STREAM, 0);
    int sc = send(st, key, strlen(key), 0);
    int sockfd;
    char ready[] = "(READY)";

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 1;
    }

    if (send(sockfd, key, strlen(key), 0) < 0) {
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        return 1;
    } else {
        if (send(sockfd, ready, strlen(ready), 0) < 0) {
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
            return 1;
        }
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
            break;
        }

        printf("client receive:%s\n", s);
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
        memset(beat, 0, sizeof(beat));

        if (send(sockfd, beat, strlen(beat), 0) < 0) {
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
            return 1;
        }

        sleep(1);
    }

    heart_signal = 0;
    return 0;
}
int start_read_thread() {
    std::thread t1(_read);
    t1.join();
    return 0;
}

int finish_read_thread() {
    heart_signal = 1;
    return 0;
}

int start_heartbeat_thread() {
    std::thread t2(heartbeat);
    t2.join();
    return 0;
}

int finish_heartbeat_thread() {
    heart_signal = 1;
    return 0;
}

int disconnect() {
    return 0;
}
int get_server_data(char *buf) {
    //char s[1024];
    int sockfd;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 1;
    }

    //memset(buf, 0, sizeof(buf));
    int rc = recv(sockfd, buf, sizeof(buf), 0);

    if (rc <= 0) { //代表socket被关闭（0）或者出错（-1）
        printf("socket disconnect!");
        return 1;
    }

    printf("client receive:%s\n", buf);

    return 0;
}

int send_operating(enum move_operating move_op, bool is_fire) {//??
   /* char s[1024];
    int sockfd;

    if (move_op == 0 && is_fire) {}

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        return 1;
    }

    int sc = send(sockfd, s, strlen(s), 0);*/
    return 0;
}
