#include "udp_util.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>

#include "utils.h"
#include "net_util.h"

#define UDP_BUFFER_SIZE 1024

int udp_listen(int port) {
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (fd == -1) {
        printf("Create UDP Socket Failed\n");
        return -1;
    }
    if ((bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr))) == -1) {
        printf("UDP Bind Failed\n");
        return -2;
    }
    if (fd > 0) {
        printf("UDP listening on port %d...\n", port);
    }
    return fd;
}

int udp_join_broadcast_group(int socket, const char* broadcast_address) {
    struct ip_mreq imreq;
    memset(&imreq, 0, sizeof(struct ip_mreq));
    imreq.imr_multiaddr.s_addr = inet_addr(broadcast_address);
    imreq.imr_interface.s_addr = INADDR_ANY;
    int status = setsockopt(socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const void*)&imreq, sizeof(struct ip_mreq));
    if (status == -1) {
        printf("UDP join broadcast group failed");
    }
    return status;
}

struct udp_recv_result udp_recv(int fd) {
    struct udp_recv_result urr;
    socklen_t addr_length = sizeof(urr.addr);
    char buffer[UDP_BUFFER_SIZE];
    bzero(buffer, UDP_BUFFER_SIZE);
    if (recvfrom(fd, buffer, UDP_BUFFER_SIZE, 0, (struct sockaddr*)&urr.addr, &addr_length) != -1) {
        bzero(urr.data, UDP_DATA_MAX_SIZE + 1);
        strncpy(urr.data, buffer, strlen(buffer) > UDP_DATA_MAX_SIZE ? UDP_DATA_MAX_SIZE : strlen(buffer));
        char* t = current_timestamp();
        printf("[%s] [%s] -> %s\n", t, sockaddr2ip(urr.addr), urr.data);
        free(t);
    }
    return urr;
}

int udp_send(int fd, struct sockaddr_in addr, char* data) {
    char buffer[UDP_BUFFER_SIZE];
    bzero(buffer, UDP_BUFFER_SIZE);
    strncpy(buffer, data, strlen(data) > UDP_BUFFER_SIZE ? UDP_BUFFER_SIZE : strlen(data));
    int result = sendto(fd, buffer, UDP_BUFFER_SIZE, 0, (struct sockaddr*)&addr, sizeof(addr));
    if (result != -1) {
        char* t = current_timestamp();
        printf("[%s] [%s] <- %s\n", t, sockaddr2ip(addr), data);
        free(t);
        return 0;
    }
    printf("send to %s failed: %d\n", sockaddr2ip(addr),  result);
    return -1;
}
