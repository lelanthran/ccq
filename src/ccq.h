
#ifndef H_CCQ
#define H_CCQ

#include <unistd.h>

struct ccq_t {
   int fds[2];
};

typedef struct ccq_t ccq_t;

#ifdef __cplusplus
#define QUALIFIER extern "C" static inline
#else
#define QUALIFIER static
#endif

QUALIFIER
int ccq_init (ccq_t *dst)
{
   return pipe (dst->fds);
}

QUALIFIER
void ccq_close (ccq_t *ccq)
{
   close (ccq->fds[0]);
   close (ccq->fds[1]);
}

QUALIFIER
int ccq_post (ccq_t *ccq, void *ptr)
{
   return write (ccq->fds[1], &ptr, sizeof ptr);
}

QUALIFIER
void *ccq_wait (ccq_t *ccq)
{
   void *ret;
   read (ccq->fds[0], &ret, sizeof ret);
   return ret;
}


#endif


