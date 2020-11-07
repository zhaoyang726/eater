#include "modules/network.h"
#include "utils/assert.h"
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <thread>
#include <atomic>
#include <mutex>

const char *data =
    "[MAP C! wwwwwwww00000000000000000]"
    "[LOCATION C! 0 1 2 3 4 5 6 7]"
    "[SCORE 0 0 0 0 0 0 0 0]";


int main() {
    int ret;
    ret = connect(0x7f000001, 9000);
    assert(ret == 1);
    std::thread server_thread([]() ->void {
        int listen_fd, ret;
        struct sockaddr_in server_addr;
        char buf[128];

        listen_fd = socket(AF_INET, SOCK_STREAM, 0);

        if (listen_fd == -1) {
            perror("socket");
            return;
        }

        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(9000);

        ret = bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (ret == -1) {
            perror("bind");
            return;
        }

        ret = listen(listen_fd, 10);

        if (ret == -1) {
            perror("listen");
            return;
        }

        int conn_fd = accept(listen_fd, NULL, NULL);

        if (conn_fd == -1) {
            perror("accept");
            return;
        }
        int n = recv(conn_fd, buf, 128, 0);
        //printf("buf ---%s\n", buf);

        assert(n > 0);
        buf[n] = 0;
        printf("%s: %d\n", __FILE__, __LINE__);

        assert(strcmp(buf, "(c96f4d7661c94cbb9706469649a7cbbc)") == 0);
        send(conn_fd, "[OK]", 4, 0);
        printf("%s: %d\n", __FILE__, __LINE__);


        sleep(1);
        send(conn_fd, "[START 1 5]", sizeof("[START 1 5]"), 0);

        n = recv(conn_fd, buf, 128, 0);
        assert(n > 0);
        buf[n] = 0;
        assert(strcmp(buf, "(READY)") == 0);
        send(conn_fd, "[OK]", 4, 0);
        sleep(1);
        printf("%s: %d\n", __FILE__, __LINE__);

        send(conn_fd, data, sizeof(data), 0);
        printf("hhh send %s\n", data);
        printf("%s: %d\n", __FILE__, __LINE__);

        close(conn_fd);
    });
    sleep(1);
    printf("%s: %d\n", __FILE__, __LINE__);
    ret = connect(0x7f000001, 9000);
    printf("%s: %d\n", __FILE__, __LINE__);
    assert(ret == 0);
    printf("%s: %d\n", __FILE__, __LINE__);
    ret = start_read_thread();
    sleep(5);
    assert(ret == 0);
    printf("%s: %d\n", __FILE__, __LINE__);
    char buf[1 << 12];
    ret = get_server_data(buf);
    assert(ret == 0);
    printf("buf is %s\n", buf);
    assert(strcmp(buf, data) == 0);
    printf("%s: %d\n", __FILE__, __LINE__);
    ret = finish_read_thread();
    assert(ret == 0);
    ret = disconnect();
    assert(ret == 0);
    printf("%s: %d\n", __FILE__, __LINE__);
    server_thread.join();
    return 0;
}
