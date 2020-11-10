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
static char *bufs;
static char flag_[10] = {0};
static std::thread *read_thread = NULL;
static std::thread *heart_thread = NULL;
static int num;
static int map;
static char *buf;
static int fd = 0;
static char *point ;


int connect(uint32_t ip_addr, uint32_t port) {
    char key[] = "(c96f4d7661c94cbb9706469649a7cbbc)";
    char s[128];

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
        return EXIT_FAILURE;
    }

    if (connect(fd, (struct sockaddr *) & addr, sizeof(addr)) == -1) {
        printf("connect fail %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    if (send(fd, key, sizeof(key), 0) < 0) {
        printf("send error :%s: %d\n", __FILE__, __LINE__);
        return EXIT_FAILURE;
    };

    if (recv(fd, s, sizeof(s), 0) < 0) {
        printf("recieve error :%s: %d\n", __FILE__, __LINE__);
        return EXIT_FAILURE;
    }

    if (!strcmp("[ERROR]", s)) {
        printf("recieve error :%s: %d\n", __FILE__, __LINE__);
        return EXIT_FAILURE;
    }

    return 0;
}


int read_data() {
    int n = 20;
    point = new char[n];

    while (!read_signal) {
        memset(point, 0, n);
        int rc = recv(fd, point, n, 0);

        if (rc <= 0) {
            printf("recieve error :%s: %d\n", __FILE__, __LINE__);
            return EXIT_FAILURE;
        }

        if (!strncmp("[OK]", point, 4)) {
            printf("%s: %d\n", __FILE__, __LINE__);
            continue;
        }

        if (!strncmp(point, "[START" , 5)) {
            sscanf(point, "[START %d %d", &num, &map);
            rc = send(fd, "(READY)", 8, 0);

            if (rc <= 0) {
                printf("recieve error :%s: %d\n", __FILE__, __LINE__);
                return EXIT_FAILURE;
            }

            delete []point;
            point = NULL;
            n = map * map + 100;
            point = new char[n];
            bufs = new char[n];

            while (!read_signal) {
                int rc = recv(fd, point, n, 0);

                if (rc <= 0) {
                    printf("recieve error :%s: %d\n", __FILE__, __LINE__);
                    return EXIT_FAILURE;
                }

                if (!strcmp("[GAMEOVER]", point)) {
                    read_signal = 1;
                    finish_heartbeat_thread();
                    delete []point;
                    delete []bufs;
                    disconnect();
                    exit(0);
                }

                strcpy(bufs, point);
            }
        }
    }

    return 0;
}
int heartbeat() {
    while (!heart_signal) {
        int rc = send(fd, "(H)", 3, 0);

        if (rc <= 0) {
            printf("socket disconnect!");
            return EXIT_FAILURE;
        }

        sleep(1);
    }

    heart_signal = 0;
    return 0;
}
int start_read_thread() {
    read_thread = new std::thread(read_data);
    return 0;
}

int finish_read_thread() {
    read_signal = 1;
    read_thread->join();
    delete read_thread;
    read_thread = NULL;
    delete []point;
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
    exit(0);
    return 0;
}
int get_server_data(char *buf) {
    strcpy(buf, bufs);
    return 0;
}

int send_operating(enum move_operating move_op, bool is_fire) {
    char s[10];
    memset(s, '\0', sizeof(s));
    char move = ' ';
    char fire = ' ';

    if (is_fire) {
        fire = 'v';
    }

    switch (move_op) {
        case move_op_stay:
            move = ' ';
            break;

        case move_op_up:
            move = 'w';
            break;

        case move_op_down:
            move = 'a';
            break;

        case move_op_left:
            move = 's';
            break;

        case move_op_right:
            move = 'd';
            break;

        default:
            break;
    }

    sprintf(s, "%s%c%c", flag_, move, fire);
    int rc = send(fd, s, sizeof(s), 0);

    if (rc <= 0) {
        printf("send error :%s: %d\n", __FILE__, __LINE__);
        return EXIT_FAILURE;
    }

    return 0;
}
