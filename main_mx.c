#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <assert.h>

#include "simple_mutex.h"
#include <pthread.h>

struct simplefu_mutex mx = SIMPLEFU_MUTEX_INITIALIZER; 
int count = 0;

void *fn(void *p)
{
  int val, i;
  for(i = 0; i < 10; ++i)
  {
    simplefu_mutex_lock(&mx);
    val = count;
    sleep(1);
    count = val + 1;
    simplefu_mutex_unlock(&mx);
  }
  return 0;
}

int main()
{ 
  pthread_t thr;
  pthread_create(&thr, NULL, fn, NULL);
  fn(NULL);
  pthread_join(thr, NULL);
  printf("%d\n", count); 
  return 0;
}
  
  
  
  
