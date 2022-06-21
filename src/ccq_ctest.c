
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <pthread.h>

#include "util.h"
#include "thread.h"

int main (int argc, char **argv)
{
   int ret = EXIT_FAILURE;

   int loopcount = 100;
   int num_producers = 3;
   int num_consumers = 3;

   (void)argc;
   printf ("Starting tests [%s %s]\n", argv[1], ccq_version);
   /* General idea:
    * 1. Create the pipe for output from consumers, start thread with that fd
    * 2. Start N producers with M loops, and P consumers with max M loops
    * 3. Wait for output on comms pipe, and send it straight to stdout
    * 4. Wait until all consumers have ended (pthread_join)
    */
   int textpipe[2];

   if ((pipe (textpipe))!=0) {
      fprintf (stderr, "Failed to create pipe\n");
      goto cleanup;
   }

   pthread_t printer_tid;
   if ((pthread_create (&printer_tid, NULL, &thread_printer, &textpipe[1]))!=0) {
      fprintf (stderr, "Failed to start printer thread\n");
      goto cleanup;
   }
   // Force context switch so that printer is reader before any consumer is
   // started.
   sleep (1);

   // Initialise the structures
   struct thread_consumer_param_t *consumer_params =
      calloc (num_consumers, sizeof *consumer_params);

   struct thread_producer_param_t *producer_params =
      calloc (num_producers, sizeof *producer_params);

   if (!consumer_params || !producer_params) {
      fprintf (stderr, "Outof memory error\n");
      goto cleanup;
   }

   int queue[2];
   if ((pipe (queue))!=0) {
      fprintf (stderr, "Pipe failure: %m\n");
      goto cleanup;
   }

   for (int i=0; i<num_consumers; i++) {
      consumer_params[i].in_fd = queue[0];
      consumer_params[i].out_fd = textpipe[1];
      // Start consumer thread
   }

   sleep (1);

   for (int i=0; i<num_producers; i++) {
      producer_params[i].out_fd = queue[1];
      // Start producer thread
   }

   // Wait for all producers to end.
   for (int i=0; i<num_producers; i++) {
      pthread_join (producer_params[i].tid, NULL);
   }


   ret = EXIT_SUCCESS;

cleanup:

   return ret;
}
