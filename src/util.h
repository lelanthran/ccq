
#ifndef H_UTIL
#define H_UTIL

#include <stdint.h>

typedef struct {
   uint64_t ctime;
   uint64_t mtime;
   uint64_t etime;
} util_t;

#ifdef __cplusplus
extern "C" {
#endif

   util_t *util_new (void);
   void util_update (util_t *util);
   void util_final (util_t *util);


#ifdef __cplusplus
};
#endif


#endif


