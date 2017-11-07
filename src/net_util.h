#ifndef net_util_h
#define net_util_h

#ifdef __cplusplus
extern "C" {
#endif

#include <netinet/in.h>

struct lan_info {
    char* local_ip;
    char* subnet_mask;
    char* broadcast_address;
    char* gateway_address;
};

struct ip_list {
    char **ptr;
    int num;
};

char* local_mac_address();

struct sockaddr_in ip2sockaddr(char* ip, int port);

char* sockaddr2ip(struct sockaddr_in sa);

#ifndef __ANDROID__
struct lan_info get_lan_info();
#endif

struct ip_list lan_ip_list(char* lan_ip, char* subnet_mask);

#ifdef __cplusplus
}
#endif

#endif
