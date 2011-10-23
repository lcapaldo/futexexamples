#include "simple_mutex.h"
void simplefu_mutex_init(simplemu mx)
{
  mx->sema.avail = 1;
  mx->sema.waiters = 0;
}


void simplefu_mutex_lock(simplemu mx)
{
  simplefu_down(&mx->sema);
}

void simplefu_mutex_unlock(simplemu mx)
{
  simplefu_up(&mx->sema);
}


