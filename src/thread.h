
#ifndef H_THREAD
#define H_THREAD

#include <stdio.h>

#include <pthread.h>

struct thread_producer_param_t {
   int out_fd;
   size_t counter;
   pthread_t tid;
};

struct thread_consumer_param_t {
   int in_fd;
   int out_fd;
   size_t counter;
   pthread_t tid;
};

#ifdef __cplusplus
extern "C" {
#endif

   void *thread_producer (void *param);   // Takes thread_producer_param_t
   void *thread_consumer (void *param);   // Takes thread_consumer_param_t
   void *thread_printer (void *param);    // Takes a single fd,cast to intptr_t


#ifdef __cplusplus
};
#endif


#endif


