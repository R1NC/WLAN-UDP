#include "net_util.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#ifndef __ANDROID__
#include <ifaddrs.h>
#include <sys/sysctl.h>
#endif

#ifdef __linux__
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <net/route.h>
#endif

#ifdef __APPLE__
#include "Apple/route.h"
#endif

#define CTL_NET 4 /* network, see socket.h */

#define ROUNDUP(a) ((a) > 0 ? (1 + (((a) - 1) | (sizeof(long) - 1))) : sizeof(long))

#define addr2char(addr) inet_ntoa(((struct sockaddr_in*)addr)->sin_addr)

#define format_ip(format, var0, var1, var2, var3) do {\
char* result = malloc(15 * sizeof(char));\
sprintf(result, format, var0, var1, var2, var3);\
ips.ptr[index] = result;\
index++;\
} while(0);

char (*ip2parts(const char* ip))[4];
int part2int(char* part);
int c2i(char ch);
#ifndef __ANDROID__
unsigned char* gateway_addr(in_addr_t *addr);
#endif

char* local_mac_address() {
#ifdef __linux__
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ - 1);
    ioctl(fd, SIOCGIFHWADDR, &ifr);
    close(fd);
    unsigned char* mac = (unsigned char*)ifr.ifr_hwaddr.sa_data;
    char format[] = "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x";
    char* result = malloc(17 * sizeof(char));
    sprintf(result, format, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return result;
#elif __APPLE__
    return "00:00:00:00:00:00";
#endif
}

struct sockaddr_in ip2sockaddr(char* ip, int port) {
    struct sockaddr_in addr;
    inet_pton(AF_INET, ip, &(addr.sin_addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    return addr;
}

char* sockaddr2ip(struct sockaddr_in sa) {
    char* ip = malloc(15 * sizeof(char));
    inet_ntop(AF_INET, &(sa.sin_addr), ip, INET_ADDRSTRLEN);
    return ip;
}

#ifndef __ANDROID__
struct lan_info get_lan_info() {
    struct lan_info li = {};
    struct ifaddrs *interfaces = NULL, *temp_addr = NULL;
    if (getifaddrs(&interfaces) == 0) {
        temp_addr = interfaces;
        while (temp_addr != NULL) {
            if (temp_addr->ifa_addr->sa_family == AF_INET) {
                if (str_eq(temp_addr->ifa_name, "en0")) {
                    li.local_ip = malloc(15 * sizeof(char));
                    strcpy(li.local_ip, addr2char(temp_addr->ifa_addr));
                    li.subnet_mask = malloc(15 * sizeof(char));
                    strcpy(li.subnet_mask, addr2char(temp_addr->ifa_netmask));
                    li.broadcast_address = malloc(15 * sizeof(char));
                    strcpy(li.broadcast_address, addr2char(temp_addr->ifa_dstaddr));
                    if (temp_addr->ifa_addr) {
                        li.gateway_address = malloc(15 * sizeof(char));
                        char* ga = gateway_addr(temp_addr->ifa_addr);
                        strcpy(li.gateway_address, ga);
                        free(ga);
                    }
                }
            }
            temp_addr = temp_addr->ifa_next;
        }
    }
    freeifaddrs(interfaces);
    return li;
}

unsigned char* gateway_addr(in_addr_t *addr) {
    unsigned char * gateway = malloc(4);
    int mib[] = {CTL_NET, PF_ROUTE, 0, AF_INET, NET_RT_FLAGS, RTF_GATEWAY};
    size_t l;
    char *buf, *p;
    struct rt_msghdr *rt;
    struct sockaddr *sa;
    struct sockaddr *sa_tab[RTAX_MAX];
    int i;
    if (sysctl(mib, sizeof(mib) / sizeof(int), 0, &l, 0, 0) < 0) {
        return gateway;
    }
    if (l > 0) {
        buf = malloc(l);
        if (sysctl(mib, sizeof(mib) / sizeof(int), buf, &l, 0, 0) < 0) {
            return gateway;
        }
        for (p = buf; p < buf + l; p += rt->rtm_msglen) {
            rt = (struct rt_msghdr*)p;
            sa = (struct sockaddr*)(rt + 1);
            for (i = 0; i < RTAX_MAX; i++) {
                if (rt->rtm_addrs & (1 << i)) {
                    sa_tab[i] = sa;
                    sa = (struct sockaddr*)((char*)sa + ROUNDUP(sa->sa_len));
                } else {
                    sa_tab[i] = NULL;
                }
            }
            if(((rt->rtm_addrs & (RTA_DST|RTA_GATEWAY)) == (RTA_DST|RTA_GATEWAY))
               && sa_tab[RTAX_DST]->sa_family == AF_INET
               && sa_tab[RTAX_GATEWAY]->sa_family == AF_INET) {
                for (int i = 0; i < 4; i++) {
                    gateway[i] = (((struct sockaddr_in*)(sa_tab[RTAX_GATEWAY]))->sin_addr.s_addr >> (i * 8)) & 0xFF;
                }
            }
        }
        free(buf);
    }
    return gateway;
}
#endif

struct ip_list lan_ip_list(char* lan_ip, char* subnet_mask) {
    struct ip_list ips = {};
    if (lan_ip != NULL && subnet_mask != NULL) {
        char (*ip_parts)[4] = ip2parts(lan_ip);
        char (*mask_parts)[4] = ip2parts(subnet_mask);
        int mask0 = part2int(mask_parts[0]), mask1 = part2int(mask_parts[1]), mask2 = part2int(mask_parts[2]), mask3 = part2int(mask_parts[3]);
        if (mask0 < 255) {
            ips.num = (254 - mask0) * 254 * 254 * 254;
            ips.ptr = malloc(ips.num * sizeof(char*));
            int index = 0;
            for (int i = mask0 + 1; i < 255; i++) {
                for (int j = 1; j < 255; j++) {
                    for (int k = 1; k < 255; k++) {
                        for (int x = 1; x < 255; x++) {
                            format_ip("%d.%d.%d.%d", i, j, k, x);
                        }
                    }
                }
            }
        } else if (mask1 < 255) {
            ips.num = (254 - mask1) * 254 * 254;
            ips.ptr = malloc(ips.num * sizeof(char*));
            int index = 0;
            for (int i = mask1 + 1; i < 255; i++) {
                for (int j = 1; j < 255; j++) {
                    for (int k = 1; k < 255; k++) {
                        format_ip("%s.%d.%d.%d", ip_parts[0], i, j, k);
                    }
                }
            }
        } else if (mask2 < 255) {
            ips.num = (254 - mask2) * 254;
            ips.ptr = malloc(ips.num * sizeof(char*));
            int index = 0;
            for (int i = mask2 + 1; i < 255; i++) {
                for (int j = 1; j < 255; j++) {
                    format_ip("%s.%s.%d.%d", ip_parts[0], ip_parts[1], i, j);
                }
            }
        } else if (mask3 < 255) {
            ips.num = 254 - mask3;
            ips.ptr = malloc(ips.num * sizeof(char*));
            int index = 0;
            for (int i = mask3 + 1; i < 255; i++) {
                format_ip("%s.%s.%s.%d", ip_parts[0], ip_parts[1], ip_parts[2], i);
            }
        }
        free(ip_parts);
        free(mask_parts);
    }
    return ips;
}

char (*ip2parts(const char* ip))[4] {
    char (*parts)[4] = malloc(4 * sizeof(char*));
    //memset(parts, 0, 4);
    int part_index = 0, last_split_index = -1;
    unsigned long len = strlen(ip);
    for (int i = 0; i < len && part_index < 4; i++) {
        int len = 0;
        if (ip[i] == '.') {
            len = i - last_split_index - 1;
        } else if (i == len - 1) {
            len = i - last_split_index;
        }
        if (len > 0) {
            strncpy(parts[part_index], ip + last_split_index + 1, len);
            last_split_index = i;
            part_index++;
        }
    }
    return parts;
}

int part2int(char* part) {
    unsigned long len = strlen(part);
    if (len == 1) {
        return c2i(part[0]) * 1;
    } else if (len == 2) {
        return c2i(part[0]) * 10 + c2i(part[1]) * 1;
    } else if (len == 3) {
        return c2i(part[0]) * 100 + c2i(part[1]) * 10 + c2i(part[2]) * 1;
    } else {
        return 0;
    }
}

int c2i(char ch) {
    return ch - '0';
}
