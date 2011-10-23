#include "ptrmvar32.h"
#include <memory>

template<typename T>
class MVar
{
  ptrmvar32 m_impl;
  
  public:
  MVar() : m_impl()
  {}

  MVar(const T& val) : m_impl(static_cast<void*>(new T(val)))
  {}

  void put(const T& val)
  {
    T* p = new T(val);
    m_impl.put(static_cast<void*>(p));
  }

  T take()
  {
    T *p = static_cast<T*>(m_impl.take());
    T r(*p);
    delete p;
    return r;
  }

private:
  template<typename F, typename V>
  struct AdapterF 
  {
    F f;
    std::auto_ptr<V>* mem;
    V** old_val;
    void* operator()(void *in)
    {
      V r = f(*(*old_val = static_cast<V*>(in)));
      mem->reset(new V(r));
      return static_cast<void*>(mem->get());
    }
  };

public:
  template<typename F>
  T pure_modify(F f)
  {
    std::auto_ptr<T> mem;
    T* old_val; // not an auto_ptr becase if we throw we have not taken ownership
               // We either threw from user code, or from new. And throwing user
               // user code would be wrong anyway (impure)
    AdapterF<F,T> adapter;
    adapter.f = f;
    adapter.mem = &mem;
    adapter.old_val = &old_val;
    m_impl.pure_modify(adapter);
    delete old_val;
    return *mem.release();
  }

  template<typename F>
  T haskell_modify(F f)
  {
     T val = take();
     struct ValRestorer {
        T val;
        bool restore;
        MVar<T> *self;
        ~ValRestorer() { if(restore) self->put(val); }
        ValRestorer(T val_, MVar<T> *self_) : val(val_), self(self_), restore(true) {}
        void Dismiss() { restore = false; }
     } restorer(val, this);
     T new_val = f(val);
     put(new_val);
     restorer.Dismiss();
  }
  
  ~MVar()
  {
    delete static_cast<T*>(m_impl.unsafe_value());
  }
};

