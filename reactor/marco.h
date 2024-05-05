#ifndef __MARCO__
#define __MARCO__
#include <stdio.h>
#define ERROR_CHECK(ret, retval, msg)                                          \
    {                                                                          \
        if (ret != retval) {                                                   \
            perror(msg);                                                       \
            return;                                                            \
        }                                                                      \
    }
#endif
