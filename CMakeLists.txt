cmake_minimum_required(VERSION 2.8)
project(UDP)

set(EXECUTABLE_OUTPUT_PATH bin)

set(SERVER_EXE_FILES 
src/main_server.c
src/udp_server.c
src/udp_util.c
src/net_util.c
src/utils.c
)
ADD_EXECUTABLE(UDPServerEXE ${SERVER_EXE_FILES})

set(CLIENT_EXE_FILES 
src/main_client.c
src/udp_client.c
src/udp_util.c
src/net_util.c
src/utils.c
)
ADD_EXECUTABLE(UDPClientEXE ${CLIENT_EXE_FILES})

set(SERVER_LIB_FILES
src/udp_server.c
src/udp_util.c
src/net_util.c
src/utils.c
)
add_library(UDPServer STATIC ${SERVER_LIB_FILES})

set(CLIENT_LIB_FILES
src/udp_client.c
src/udp_util.c
src/net_util.c
src/utils.c
)
add_library(UDPClient STATIC ${CLIENT_LIB_FILES})