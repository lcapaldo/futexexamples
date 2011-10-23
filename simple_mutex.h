#include "simple_futex.h"
#ifdef __cplusplus
extern "C" {
#endif
struct simplefu_mutex 
{
  struct simplefu_semaphore sema;
};

typedef struct simplefu_mutex *simplemu;

#define SIMPLEFU_MUTEX_INITIALIZER { {1, 0} }

void simplefu_mutex_init(simplemu mx);
void simplefu_mutex_lock(simplemu mx);
void simplefu_mutex_unlock(simplemu mx);
#ifdef __cplusplus
}
#endif
