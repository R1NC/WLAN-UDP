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
void run_udp_client_request_all_with_ip_mask(int req, struct lan_info* li);
void run_udp_client_request_all(int req);
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

void udp_client_request(const char* lan_ip, int req) {
    if (!listening_udp_client) return;
    struct sockaddr_in addr = ip2sockaddr(lan_ip, UDP_SERVER_PORT);
    char* rq = NULL;
    switch (req) {
        case REQ_MAC_ADDRESS: {
            rq = STR_REQ_MAC_ADDRESS;
            break;
        }
    }
    if (rq) {
        char* data = (char*)malloc(strlen(rq) * sizeof(char));
        strcpy(data, rq);
        udp_send(udp_client_socket_fd, addr, data);
        free(data);
    }
}

void udp_client_request_all_with_ip_mask(int req, const char* lan_ip, const char* subnet_mask) {
    pthread_t req_thread;
    struct lan_info li = {.local_ip = lan_ip, .subnet_mask = subnet_mask};
    pthread_create(&req_thread, NULL, run_udp_client_request_all_with_ip_mask, &li);
}

void run_udp_client_request_all_with_ip_mask(int req, struct lan_info* li) {
    if (li != NULL && li->local_ip != NULL && li->subnet_mask != NULL) {
        struct ip_list ips = lan_ip_list(li->local_ip, li->subnet_mask);
        for (int i = 0; i < ips.num; i++) {
            if (!str_eq(li->local_ip, ips.ptr[i])) {
                udp_client_request(ips.ptr[i], req);
            }
        }
        free(ips.ptr);
    } else {
        printf("fetch LAN info failed.\n");
    }
}

void udp_client_request_all(int req) {
    pthread_t req_thread;
    pthread_create(&req_thread, NULL, run_udp_client_request_all, req);
}

void run_udp_client_request_all(int req) {
    /*struct lan_info li = get_lan_info();
    run_udp_client_request_all_with_ip_mask(req, &li);
    if (li.local_ip != NULL) {
        free(li.local_ip);
    }
    if (li.subnet_mask != NULL) {
        free(li.subnet_mask);
    }*/
    udp_client_request(UDP_BROADCAST_ADDRESS, req);
}
