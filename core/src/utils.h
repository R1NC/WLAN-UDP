#ifndef utils_h
#define utils_h

#ifdef __cplusplus
extern "C" {
#endif

char* current_timestamp();

int str_eq(const char* src, const char* dst);

int str_pre(const char* src, const char* pre);

#ifdef __cplusplus
}
#endif

#endif
