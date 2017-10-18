#ifndef udp_server_h
#define udp_server_h

#ifdef __cplusplus
extern "C" {
#endif

void udp_server_start(const char* device_info_json);

void udp_server_stop();

#ifdef __cplusplus
}
#endif

#endif
