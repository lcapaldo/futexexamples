#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <assert.h>

#include "simple_futex.h"

int main()
{
  int lockfile = open("ipc_lock", O_RDWR);
  assert(lockfile != -1);
  simplefu sema = mmap(NULL, sizeof(*sema), PROT_READ|PROT_WRITE,
    MAP_SHARED, lockfile, 0);
  assert(sema != MAP_FAILED);
  
  int pid = fork();
  assert(pid != -1);
  if( pid == 0 ) { // child
    printf("Initializing...\n");
    sleep(10);
    printf("Done initializing\n");
    simplefu_up(sema);
    exit(0);
  }
  printf("Waiting for child...\n");
  simplefu_down(sema);
  printf("Child done initializing\n");
  
  return 0;
}
  
  
  
  
