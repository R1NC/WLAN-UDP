#include "udp_client.h"
#include <stdio.h>

void cb(const char* ip, int req, char* data) {
    printf("ip:%s req:%d data:%s\n", ip, req, data);
}

int main(int argc, char** argv) {
    udp_client_start(cb);
    sleep(3);
#ifndef __ANDROID__
    udp_client_request_all(REQ_DEVICE_INFO);
#endif
    while(1) {};
    return 0;
}
