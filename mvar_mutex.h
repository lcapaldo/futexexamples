#include "ptrmvar32.h"

class mutex : private ptrmvar32
{
  public:
  mutex(); 
  void lock();
  void unlock();
};

