#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
#include <time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#elif __APPLE__
#include <sys/time.h>
#endif

char* current_timestamp() {
    time_t timer;
    char* ymdhms[26];
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    strftime(ymdhms, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    
    struct timeval tval;
    gettimeofday(&tval, NULL);
    long int usec = (long int)tval.tv_usec;

    char format[] = "%s.%06ld";
    char* result = (char*)malloc(33 * sizeof(char));
    sprintf(result, format, ymdhms, usec);
    
    return result;
}

int str_eq(const char* src, const char* dst) {
    return src && dst && strcmp(src, dst) == 0;
}

int str_pre(const char *str, const char *pre) {
    size_t lenpre = strlen(pre), lenstr = strlen(str);
    return lenstr <= lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}
