
#ifndef H_CCQCPP
#define H_CCQCPP

#include <string>

#include <unistd.h>

template<class T> class ccq {
   private:
      int fds[2];

   public:
      ccq () {
         if (pipe (this->fds) != 0) {
            throw 1;
         }
      }

      ~ccq () {
         close (this->fds[0]);
         close (this->fds[1]);
      }

      int post (T *ptr) {
         return write (this->fds[1], ptr, sizeof ptr);
      }

      T *wait (void) {
         T *ret;
         read (this->fds[0], &ret, sizeof ret);
         return ret;
      }
};


#endif


