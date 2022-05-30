
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include"util.h"
#include "thread.h"

void *thread_producer (void *param)
{
   struct thread_producer_param_t *p = param;

   for (size_t i=0; i<p->count; i++) {
      util_t *instance = util_new ();
      if ((write (p->out_fd, &instance, sizeof instance)) != sizeof instance) {
         fprintf (stderr, "Write failure producer [%zu], aborting thread\n", i);
         return NULL;
      }
   }
   return NULL;
}


void *thread_consumer (void *param)
{
   struct thread_consumer_param_t *p = param;
   util_t *incoming;

   size_t count = 0;

   while ((read (p->in_fd, &incoming, sizeof incoming)) > 0 && incoming) {
      util_update (incoming);
      // Change this to printf.
      if ((fprintf (p->out_fp, "%zu, %zu, %zu\n",
                     count, incoming->ctime, incoming->mtime)) < 0) {
         fprintf (stderr, "Write failure (consumer) [%zu], aborting thread\n",
                  count);
         return NULL;
      }
      count++;
   }

   return NULL;
}


