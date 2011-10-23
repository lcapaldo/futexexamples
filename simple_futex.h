#ifdef __cplusplus
extern "C" {
#endif
struct simplefu_semaphore {
  int avail;
  int waiters;
};

typedef struct simplefu_semaphore *simplefu;

void simplefu_down(simplefu who);
void simplefu_up(simplefu who);


#ifdef __cplusplus
}
#endif
