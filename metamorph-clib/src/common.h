#ifndef COMMON_GLOB
#define COMMON_GLOB

#define TRUE 1
#define FALSE 0

typedef int bool_t;
void error(int);

#define CRASH(CODE) longjmp(error_buffer,CODE);

#endif