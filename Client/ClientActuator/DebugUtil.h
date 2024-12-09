#ifndef DEBUGUTIL_H
#define DEBUGUTIL_H

#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
    #define _DEBUG 1
    #define ASSERT(exp, msg) \
    { \
        if (!(exp)) { \
            fprintf(stderr, " Assertion failed: %s\n", #exp); \
            fprintf(stderr, " Message : %s\n", msg); \
            fprintf(stderr, " in file %s\n", __FILE__); \
            fprintf(stderr, " in function %s\n", __func__); \
            fprintf(stderr, " at line %d\n", __LINE__); \
            exit(-1); \
        } \
    }
#else
    #define _DEBUG 0
    #define ASSERT(exp, msg)
#endif

#ifdef TEST
    #define _TEST 1
#else 
    #define _TEST 0
#endif

#endif