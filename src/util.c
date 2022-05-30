#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "util.h"

static const uint64_t num_ms_in_sec = 1000ULL;

static uint64_t util_clock_now (void)
{
   struct timespec rt;
   uint64_t now;

   if (clock_gettime (CLOCK_MONOTONIC_COARSE, &rt)!=0)
      return (uint64_t)-1;

   now = rt.tv_sec * num_ms_in_sec;
   now += rt.tv_nsec / (uint64_t)1000000;

   return now;
}

util_t *util_new (void)
{
   util_t *ret = malloc (sizeof *ret);
   if (!ret) {
      return NULL;
   }

   ret->ctime = util_clock_now ();

   return ret;
}

void util_update (util_t *util)
{
   util->mtime = util_clock_now ();
}

void util_final (util_t *util)
{
   util->etime = util_clock_now ();
}

