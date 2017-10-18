#include "udp_server.h"

static const char* json = "{\
    \"manufacturer\": \"LENOVO\",\
    \"deviceName\": \"MOTO G\",\
    \"brandIconUrl\": \"https://rinc.xyz/img/favicon.png\",\
    \"productId\": \"TVS9876543210\",\
    \"DSN\": \"9876543210\",\
    \"MAC\": \"a0:b1:c2:d3:e4:f6\",\
    \"OS\": \"Android 5.1\"\
  }";

int main(int argc, char** argv) {
    udp_server_start(json);
    while(1) {};
    return 0;
}
