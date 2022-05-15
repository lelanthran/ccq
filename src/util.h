
#ifndef H_UTIL
#define H_UTIL

#include <stdint.h>

typedef struct {
   uint64_t ctime;
} util_t;

#ifdef __cplusplus
extern "C" {
#endif

   util_t *util_new (void);


#ifdef __cplusplus
};
#endif


#endif


