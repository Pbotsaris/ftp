#ifndef FUNCTION_WRAPPER_H
#define FUNCTION_WRAPPER_H
#include <memory>

/*
 *  This class is a wrapper around a package_task object to make it movable to the ThreadSafeQueue. 
 *  See ThreadPoll::submit().
 */

namespace threads {
class MovableWrapper {

  struct BaseImpl {      /* using inheritance to make the wrapper polymorphic */
    virtual void call() = 0;
    virtual ~BaseImpl(){};
  };

  std::unique_ptr<BaseImpl> m_impl;

  template <typename FuncType> struct ImplType : BaseImpl {
    FuncType m_func;

    ImplType(FuncType &&t_func) : m_func(std::move(t_func)) {}
    void call() { m_func(); }
  };

public:
  template <typename FuncType>
  MovableWrapper(FuncType &&t_func)
      : m_impl(new ImplType<FuncType>(std::move(t_func))) {}

  void operator()(){ m_impl->call(); } // call func in internal implementation

  MovableWrapper() = default;

  /* move only */
  MovableWrapper(MovableWrapper &&t_other)
    :m_impl(std::move(t_other.m_impl)) {}

  MovableWrapper& operator=(MovableWrapper &&t_other){
    m_impl = std::move(t_other.m_impl);
    return *this;
  }

  /* no copy */
  MovableWrapper(const MovableWrapper&) = delete; 
  MovableWrapper(MovableWrapper&) = delete;
  MovableWrapper& operator=(const MovableWrapper&) = delete;

};

} // namespace threads

#endif // !MOVABLE_WRAPPER_H
