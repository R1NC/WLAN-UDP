#ifndef udp_util_h
#define udp_util_h

#ifdef __cplusplus
extern "C" {
#endif

#include <netinet/in.h>

#define UDP_DATA_MAX_SIZE 512

struct udp_recv_result {
    struct sockaddr_in addr;
    char data[UDP_DATA_MAX_SIZE + 1];
};

int udp_listen(int port);

struct udp_recv_result udp_recv(int fd);

int udp_send(int fd, struct sockaddr_in addr, char* data);

#ifdef __cplusplus
}
#endif

#endif
