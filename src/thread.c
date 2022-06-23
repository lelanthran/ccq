
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include"util.h"
#include "thread.h"
#include "ccq.h"

void spurious_warnings_suppression (void)
{
   // Spurious warnings suppression
   (void)ccq_init;
   (void)ccq_shutdown;
   (void)ccq_wait;
   (void)ccq_post;
}


void *thread_producer (void *param)
{
   struct thread_producer_param_t *p = param;

   for (size_t i=0; i<p->maxcount; i++) {
      util_t *instance = util_new ();
      if ((ccq_post (p->message_queue, instance)) != 0) {
         fprintf (stderr, "Write failure (producer): [%zu], aborting thread\n", i);
         return NULL;
      }
   }
   return NULL;
}


void *thread_consumer (void *param)
{
   struct thread_consumer_param_t *p = param;
   util_t *incoming;
   char output[100];

   size_t count = 0;

   while ((incoming = ccq_wait (p->message_queue)) !=0 && count < p->maxcount) {

      util_update (incoming);

      snprintf (output, sizeof output, "%zu, %zu, %zu\n",
                count, incoming->ctime, incoming->mtime);

      if ((ccq_post (p->printer_queue, output)) < 0) {
         fprintf (stderr, "Write failure (consumer) [%zu], aborting thread\n",
                  count);
         return NULL;
      }
      count++;
      free (incoming);
   }

   return NULL;
}

// Only prints messages shorter than PIPE_BUF
void *thread_printer (void *param)
{
   struct ccq_t *p = param;
   char *data = NULL;
   int stdoutfd = fileno (stdout);

   while ((data = ccq_wait (p))!= NULL) {
      write (stdoutfd, data, strlen (data));
   }

   fprintf (stderr,"Failed to read: %m\n");
   return NULL;
}

