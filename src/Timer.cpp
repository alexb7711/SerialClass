///
/// @file lib/os/Timer.hpp
///
/// This file contains the Timer class definition.
///

#include <cstdio>
#include <cstring>
#include <signal.h>
#include <time.h>
#include <Platform.h>
#include <Semaphore.h>
#include <Mutex.h>
#include <Thread.h>
#include <Timer.h>

namespace os
{

enum {TMR_DLY_TICKS = 5};

//======================================================================================================================
static Thread*        tmr_monitor(0);
static Timer::TmrList tmr_tbl;
static Mutex          mutex;

//======================================================================================================================
class Timer::Monitor : public Thread
{
  friend void TimerHandler(int sig, siginfo_t* si, void* uc);

public:
  //----------------------------------------------------------------------------
  Monitor()
    : Thread("TimerMonitor")
  {
    start();
  }

  //----------------------------------------------------------------------------
  ~Monitor()
  {
    Thread::stop();
    timer_delete(m_timerID);
  }

  //----------------------------------------------------------------------------
  bool initialize(size_t period_ms)
  {
    struct sigevent   te;
    struct sigaction  sa;
    int               sigNo = SIGRTMIN;

    /* Set up signal handler. */
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &TimerHandler;
    sigemptyset(&sa.sa_mask);

    if ( sigaction(sigNo, &sa, NULL) == -1 )
    {
      fprintf(stderr, "Error: Failed to setup signal handling for.\n");
      perror("sigaction");
      return false;
    }

    /* Set and enable alarm */
    te.sigev_notify          = SIGEV_SIGNAL;
    te.sigev_signo           = sigNo;
    te.sigev_value.sival_ptr = this;

    if ( timer_create(CLOCK_REALTIME, &te, &m_timerID) != 0 )
    {
      perror("timer_create");
      return false;
    }

    itimerspec its;
    its.it_value.tv_sec     = period_ms / 1000;
    its.it_value.tv_nsec    = (period_ms % 1000) * 1000000;
    its.it_interval.tv_sec  = 0;
    its.it_interval.tv_nsec = 0;
    its.it_interval.tv_sec  = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    if ( timer_settime(m_timerID, 0, &its, NULL) != 0 )
    {
      return false;
    }

    return true;
  }

  //----------------------------------------------------------------------------
  virtual void run()
  {
    if (!initialize(TMR_DLY_TICKS)) return;

    while (!isInterrupted())
    {
      // sleep(TMR_DLY_TICKS);
      m_signal.wait();

      mutex.lock();
      TmrList::iterator it = tmr_tbl.begin();
      mutex.unlock();

      while (it != tmr_tbl.end())
      {
        Timer* p_tmr = *(it);

        if (p_tmr->m_tmr_enabled)
        {
          if (p_tmr->m_tmr_ctr > 0)
          {
            p_tmr->m_tmr_ctr--;
          }
        }

        if (p_tmr->m_tmr_enabled)
        {
          if (p_tmr->m_tmr_ctr == 0)
          {
            if (p_tmr->m_tmr_func != 0)
            {
              p_tmr->m_tmr_func(p_tmr->m_tmr_func_arg);
            }

            p_tmr->m_tmr_enabled = (p_tmr->m_tmr_ctr = p_tmr->m_tmr_init) != 0;
          }

        }

        mutex.lock();
        it++;
        mutex.unlock();
      }
    }
  }

  //----------------------------------------------------------------------------
  static void TimerHandler(int sig, siginfo_t* si, void* uc)
  {
    Monitor* p_tmon = static_cast<Monitor*>(si->si_value.sival_ptr);
    if (p_tmon != 0)
    {
      p_tmon->m_signal.post();
    }
  }

private:
  timer_t   m_timerID;  // Timer ID
  Semaphore m_signal;   // Semaphore used for signaling waiting users
};

//======================================================================================================================
//
Timer::Timer(TmrCallback func, void* arg)
  : m_tmr_enabled(false)
  , m_tmr_ctr(0)
  , m_tmr_init(0)
  , m_tmr_func(func)
  , m_tmr_func_arg(arg)
{
  m_tmrid = tmr_tbl.end();
  init();
}

//======================================================================================================================
//
Timer::~Timer()
{
  stop();

  if (m_tmrid != tmr_tbl.end())
  {
    mutex.lock();
    tmr_tbl.erase(m_tmrid);
    mutex.unlock();
  }

  if (tmr_tbl.empty())
  {
    delete tmr_monitor;
    tmr_monitor = 0;
  }
}

//======================================================================================================================
//
void Timer::init()
{
  if (tmr_monitor == 0)
  {
    if (!tmr_tbl.empty())
    {
      tmr_tbl.erase(tmr_tbl.begin(), tmr_tbl.end());
    }

    tmr_monitor = new Monitor;
  }

  mutex.lock();
  tmr_tbl.push_back(this);
  m_tmrid = --tmr_tbl.end();
  mutex.unlock();
}

//======================================================================================================================
//
void Timer::start(size_t timeout, size_t reload)
{
  if (!m_tmr_enabled)
  {
    m_tmr_ctr  = timeout / TMR_DLY_TICKS;
    m_tmr_init = reload / TMR_DLY_TICKS;
    m_tmr_enabled = true;
  }
}

//======================================================================================================================
//
void Timer::stop()
{
  m_tmr_enabled = false;
}

//======================================================================================================================
//
size_t Timer::peek()
{
  return m_tmr_ctr;
}

}  // namespace os
