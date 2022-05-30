
#ifndef H_THREAD
#define H_THREAD

#include <stdio.h>

struct thread_producer_param_t {
   int out_fd;
   size_t count;
};

struct thread_consumer_param_t {
   int in_fd;
   FILE *out_fp;
};

#ifdef __cplusplus
extern "C" {
#endif

   void *thread_producer (void *param);   // Takes thread_producer_param_t
   void *thread_consumer (void *param);   // Takes thread_consumer_param_t


#ifdef __cplusplus
};
#endif


#endif


