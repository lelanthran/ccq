
#ifndef H_CCQ
#define H_CCQ

#include <unistd.h>

struct ccq_t {
   int fds[2];
};


#ifdef __cplusplus
#define QUALIFIER extern "C" static inline
#else
#define QUALIFIER static
#endif

QUALIFIER
int ccq_init (struct ccq_t *dst)
{
   return pipe (dst->fds);
}

QUALIFIER
void ccq_shutdown (struct ccq_t *ccq)
{
   close (ccq->fds[0]);
   close (ccq->fds[1]);
}

QUALIFIER
int ccq_post (struct ccq_t *ccq, void *ptr)
{
   return write (ccq->fds[1], &ptr, sizeof ptr);
}

QUALIFIER
void *ccq_wait (struct ccq_t *ccq)
{
   void *ret;
   if ((read (ccq->fds[0], &ret, sizeof ret)) != 0)
      return NULL;
   return ret;
}


#endif


