
#include <cstdio>

#include "concurrentqueue.h"

#include "util.h"

int main (int argc, char **argv)
{
   (void)argc;
   printf ("Starting tests [%s %s]\n", argv[1], ccq_version);
   return 0;
}
