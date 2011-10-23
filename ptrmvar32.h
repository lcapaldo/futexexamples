#ifndef PTRMVAR32_H
#define PTRMVAR32_H
#include <stdint.h>
#include <assert.h>
class ptrmvar32
{
  uintptr_t m_val;
  static char s_pointers_are_32_bits[sizeof(void*) == 4 ? 1 : -1];
  static void futex_wait(void *, uintptr_t);
  static void futex_wake(void *);
  public:
  ptrmvar32(void *initial_value) : m_val(reinterpret_cast<uintptr_t>(initial_value))
  {
    assert(m_val);
  }

  ptrmvar32() : m_val(0)
  {}

  void* take()
  {
    uintptr_t curr;
    while(1) {
      curr = m_val;
      if( curr == 0 )
      {
        if( __sync_bool_compare_and_swap(&m_val, 0, 1) )
          futex_wait(&m_val, 1);
      } else if( curr == 1 ) {
        futex_wait(&m_val, 1);
      } else {
        if( __sync_bool_compare_and_swap(&m_val, curr, 0) )
        {
          if( curr & 1 )
          {
            futex_wake(&m_val);
          }
          curr &= ~1;
          return reinterpret_cast<void*>(curr);
        }
      }
    }
  }

  void put(void *val)
  {
    const uintptr_t new_val = reinterpret_cast<uintptr_t>(val);
    uintptr_t curr;
    while(1) {
      curr = m_val;
      if( curr == 0 )
      {
        if( __sync_bool_compare_and_swap(&m_val, 0, new_val) )
          return;
      } else if( curr == 1 ) {
        if( __sync_bool_compare_and_swap(&m_val, 1, new_val) ) {
          futex_wake(&m_val);
          return;
        }
      } else {
        if( __sync_bool_compare_and_swap(&m_val, curr, curr | 1) ) {
          futex_wait(&m_val, curr | 1);
        }
      }
    }
  }

  template<typename F>
  void* pure_modify(F f)
  {
    uintptr_t curr;
    while(1) {
      curr = m_val;
      if( curr == 0 )
      {
        if( __sync_bool_compare_and_swap(&m_val, 0, 1) )
          futex_wait(&m_val, 1);
      } else if( curr == 1 ) {
        futex_wait(&m_val, 1);
      } else {
        void *new_val = f(reinterpret_cast<void*>(curr & ~1));
        if( __sync_bool_compare_and_swap(&m_val, curr, new_val) )
           break;
      }
    } 
  }

  void* unsafe_value()
  {
    return reinterpret_cast<void*>( m_val & ~1);
  }
};

         
          
      
#endif 
