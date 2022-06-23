
#ifndef H_THREAD
#define H_THREAD

#include <stdio.h>

#include <pthread.h>

#include "ccq.h"

struct thread_producer_param_t {
   struct ccq_t *message_queue;
   size_t maxcount;
   pthread_t tid;
};

struct thread_consumer_param_t {
   struct ccq_t *message_queue;
   struct ccq_t *printer_queue;
   size_t maxcount;
   pthread_t tid;
};

#ifdef __cplusplus
extern "C" {
#endif

   void *thread_producer (void *param);   // Takes thread_producer_param_t
   void *thread_consumer (void *param);   // Takes thread_consumer_param_t
   void *thread_printer (void *param);    // Takes ccq_t pointer


#ifdef __cplusplus
};
#endif


#endif


