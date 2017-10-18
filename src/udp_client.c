#include "udp_client.h"
#include "utils.h"
#include "net_util.h"
#include "udp_util.h"
#include "udp_constants.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int listening_udp_client;
int udp_client_socket_fd;
#ifndef __ANDROID__
void run_udp_client_request_all(int req);
#endif
udp_callback _udp_callback_;

void run_udp_client();

void udp_client_start(udp_callback callback) {
    if (listening_udp_client) return;
    _udp_callback_ = callback;
    listening_udp_client = 1;
    pthread_t client_thread;
    pthread_create(&client_thread, NULL, run_udp_client, NULL);
}

void run_udp_client() {
    if ((udp_client_socket_fd = udp_listen(UDP_CLIENT_PORT)) > 0) {
        while (listening_udp_client) {
            struct udp_recv_result urr = udp_recv(udp_client_socket_fd);
            char *req = NULL, *str = NULL;
            if (str_pre(urr.data, STR_RSP_MAC_ADDRESS)) {
                req = REQ_MAC_ADDRESS;
                str = STR_REQ_MAC_ADDRESS;
            }
            if (req && str && _udp_callback_) {
                char* data = urr.data + strlen(str);
                _udp_callback_(sockaddr2ip(urr.addr), req, data);
            }
        }
        close(udp_client_socket_fd);
    }
}

void udp_client_stop() {
    listening_udp_client = 0;
}

void udp_client_request(const char* ip, int req) {
    if (!listening_udp_client) return;
    struct sockaddr_in addr = ip2sockaddr(ip, UDP_SERVER_PORT);
    char* rq = NULL;
    switch (req) {
        case REQ_MAC_ADDRESS: {
            rq = STR_REQ_MAC_ADDRESS;
            break;
        }
    }
    if (rq) {
        char* data = malloc(strlen(rq) * sizeof(char));
        strcpy(data, rq);
        udp_send(udp_client_socket_fd, addr, data);
    }
}

#ifndef __ANDROID__
void udp_client_request_all(int req) {
    pthread_t req_thread;
    pthread_create(&req_thread, NULL, run_udp_client_request_all, req);
}

void run_udp_client_request_all(int req) {
    struct lan_info li = get_lan_info();
    if (li.local_ip != NULL && li.subnet_mask != NULL) {
        struct ip_list ips = lan_ip_list(li.local_ip, li.subnet_mask);
        for (int i = 0; i < ips.num; i++) {
            if (!str_eq(li.local_ip, ips.ptr[i])) {
                udp_client_request(ips.ptr[i], req);
            }
        }
        free(ips.ptr);
        free(li.local_ip);
        free(li.subnet_mask);
    } else {
        printf("fetch LAN info failed.\n");
    }
}
#endif