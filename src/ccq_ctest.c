
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <pthread.h>

#include "util.h"
#include "thread.h"
#include "ccq.h"

int main (int argc, char **argv)
{
   // Spurious warnings suppression
   (void)ccq_init;
   (void)ccq_shutdown;
   (void)ccq_wait;
   (void)ccq_post;

   int ret = EXIT_FAILURE;

   int loopcount = 100;
   int num_producers = 3;
   int num_consumers = 3;
   struct ccq_t message_queue = { { -1, -1 } };
   struct ccq_t printer_queue = { { -1, -1 } };

   (void)argc;
   printf ("Starting tests [%s %s]\n", argv[1], ccq_version);
   /* General idea:
    * 1. Create the pipe for output from consumers, start thread with that fd
    * 2. Start N producers with M loops, and P consumers with max M loops
    * 3. Wait for output on comms pipe, and send it straight to stdout
    * 4. Wait until all consumers have ended (pthread_join)
    */
   if ((ccq_init (&message_queue)) != 0 || ccq_init(&printer_queue) != 0) {
      fprintf (stderr, "Failed to initialise the queues: %m\n");
      goto cleanup;
   }

   pthread_t printer_tid;
   if ((pthread_create (&printer_tid, NULL, thread_printer, &printer_queue)) != 0) {
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


   //Create theconsumer threads
   for (int i=0; i<num_consumers; i++) {
      consumer_params[i].message_queue = &message_queue;
      consumer_params[i].printer_queue = &printer_queue;
      consumer_params[i].maxcount = loopcount;
      // Start consumer thread
   }

   sleep (1);

   // Create the producer threads
   for (int i=0; i<num_producers; i++) {
      producer_params[i].message_queue = &message_queue;
      producer_params[i].maxcount = loopcount;
      // Start producer thread
   }

   sleep (1);

   // Wait for all producers to end.
   for (int i=0; i<num_producers; i++) {
      pthread_join (producer_params[i].tid, NULL);
   }

   // Kill all the consumers
   for (int i=0; i<num_consumers; i++) {
      ccq_post (&message_queue, NULL);
   }

   // Wait for all consumers to end.
   for (int i=0; i<num_consumers; i++) {
      pthread_join (consumer_params[i].tid, NULL);
   }

   ret = EXIT_SUCCESS;

cleanup:
   ccq_shutdown (&message_queue);
   ccq_shutdown (&printer_queue);

   return ret;
}
