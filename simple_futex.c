#include "simple_futex.h"
#include <linux/futex.h>
#include <sys/syscall.h>

void simplefu_down(simplefu who)
{
  int val;
  do {
   val = who->avail;
   if( val > 0 && __sync_bool_compare_and_swap(&who->avail, val, val - 1) )
     return;
   __sync_fetch_and_add(&who->waiters, 1);
   syscall(__NR_futex, &who->avail, FUTEX_WAIT, val, NULL, 0, 0);
   __sync_fetch_and_sub(&who->waiters, 1);
  } while(1);
}

void simplefu_up(simplefu who)
{
  int nval = __sync_add_and_fetch(&who->avail, 1);
  if( who->waiters > 0 )
  {
     syscall(__NR_futex, &who->avail, FUTEX_WAKE, nval, NULL, 0, 0);
  }  
}
