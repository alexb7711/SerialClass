/// @file lib/os/Timer.h
///
/// Redirects the Timer class.

#ifndef TIMER_H
#define TIMER_H

#include <list>

namespace os
{

class Timer
{
  class Monitor;
  friend class Monitor;

public:
  typedef std::list<Timer*> TmrList;
  typedef void(*TmrCallback)(void*);

  /// Constructor
  ///
  /// @param[in]  func  Timer callback function
  /// @param      arg   Callback function argument
  Timer(TmrCallback func, void* arg);

  /// Destroys the timer.
  virtual ~Timer();

  /// Starts the timer
  ///
  /// @param[in]  timeout  The timeout in milliseconds
  /// @param[in]  reload   The reload in milliseconds
  void start(size_t timeout, size_t reload=0);

  /// Stops the timer
  void stop();

  /// Retrieves the timer counter
  ///
  /// @return Timer counter
  size_t peek();

private:
  Timer();
  Timer(const Timer&);
  Timer& operator=(const Timer&);
  void init();

  TmrList::iterator m_tmrid;
  bool              m_tmr_enabled;
  size_t            m_tmr_ctr;
  size_t            m_tmr_init;
  TmrCallback       m_tmr_func;
  void*             m_tmr_func_arg;
};

} // namespace os

#endif // TIMER_H
