#ifndef udp_client_h
#define udp_client_h

#ifdef __cplusplus
extern "C" {
#endif

static const int REQ_MAC_ADDRESS = 1;

typedef void(*udp_callback)(const char* ip, int req, char* data);

void udp_client_start(udp_callback callback);

void udp_client_stop();

void udp_client_request(const char* ip, int req);

#ifndef __ANDROID__
void udp_client_request_all(int req);
#endif

#ifdef __cplusplus
}
#endif

#endif
