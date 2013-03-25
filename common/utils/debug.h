#ifndef __COMMON_UTILS_DEBUG_H__
#define __COMMON_UTILS_DEBUG_H__

#ifdef NDEBUG
#define debug(...)
#else
#include <stdio.h>
#define debug(...) { fprintf(stderr, "Debug %s:%d -> ", __FILE__, __LINE__); fprintf(stderr, __VA_ARGS__); fputs("\n", stderr);\
fflush(stderr);}
#endif

#endif

