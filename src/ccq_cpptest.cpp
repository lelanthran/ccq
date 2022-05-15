
#include <cstdio>

#include "ccqcpp.h"
#include "util.h"

int main (int argc, char **argv)
{
   (void)argc;

   ccq<util_t> ccq;
   printf ("Starting tests [%s %s]\n", argv[1], ccq_version);
   return 0;
}
