
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include"util.h"
#include "thread.h"

void *thread_producer (void *param)
{
   struct thread_producer_param_t *p = param;

   for (size_t i=0; i<p->counter; i++) {
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
   char output[100];

   size_t count = 0;

   while ((read (p->in_fd, &incoming, sizeof incoming)) > 0 &&
            incoming &&
            count < p->counter) {

      util_update (incoming);

      snprintf (output, sizeof output, "%zu, %zu, %zu\n",
                count, incoming->ctime, incoming->mtime);
      size_t outputlen = strlen (output);

      if ((write (p->out_fd, output, outputlen)) < 0) {
         fprintf (stderr, "Write failure (consumer) [%zu], aborting thread\n",
                  count);
         return NULL;
      }
      count++;
      free (incoming);
   }

   return NULL;
}


void *thread_printer (void *param)
{
   int *ptr_in_fd = param;
   static char data[4196];
   ssize_t nbytes = 0;
   int stdoutfd = fileno (stdout);

   while ((nbytes = read (*ptr_in_fd, data, sizeof data)) > 0) {
      write (stdoutfd, data, nbytes);
   }

   fprintf (stderr,"Failed to read: %m\n");
   return NULL;
}
