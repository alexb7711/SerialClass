/// @file    lib/os/posix/Thread.cpp
///
/// Contains the Thread class posix implementation.
///

#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <string>
#include <Semaphore.h>
#include <Thread.h>

using interface::IRunnable;

namespace os
{

enum { DEFAULT_STACKSIZE = 4096 };
enum OsThreadPriority
{
  LOWEST_THREAD_PRIORITY      = 0,   // Lowest Priority Thread
  LOW_THREAD_PRIORITY         = 42,  // Low Priority Thread
  MEDIUM_LOW_THREAD_PRIORITY  = 84,  // Med-Low Priority Thread
  MEDIUM_THREAD_PRIORITY      = 127, // Med Priority Thread
  MEDIUM_HIGH_THREAD_PRIORITY = 169, // Med-High Priority Thread
  HIGH_THREAD_PRIORIY         = 211, // High Priority Thread
  HIGHEST_THREAD_PRIORITY     = 255  // Highest Priority Thread
};

struct Thread::OsTheadData
{
  std::string    m_name;       // Thread's name
  int            m_priority;   // Thread's priority
  uint32_t       m_stacksize;  // Thread's stack size
  IRunnable*     m_p_target;   // Thread's runnable
  State          m_state;      // Thread's state
  Semaphore      m_sync_sem;   // Thread start/join semaphore.
  pthread_attr_t m_attr;       // pthread attributes
  pthread_t      m_tid;        // pthread task ID

  OsTheadData(IRunnable* p_target, const char* name)
    : m_name(name)
    , m_priority(MEDIUM_THREAD_PRIORITY)
    , m_stacksize(DEFAULT_STACKSIZE)
    , m_p_target(p_target)
    , m_state(STOPPED)
  {
    pthread_attr_init(&m_attr);
  }

  ~OsTheadData()
  {
    pthread_attr_destroy(&m_attr);
  }

  static void* threadRun(void* p_arg)
  {
    Thread* p_inst = reinterpret_cast<Thread*>(p_arg);

    if (p_inst != 0)
    {
      Thread::OsTheadData& osThreadData = *(p_inst->m_pOsTheadData);

      osThreadData.m_state = RUNNING;
      osThreadData.m_sync_sem.post();  // signal started
      p_inst->run();
      osThreadData.m_state = STOPPED;
      osThreadData.m_sync_sem.post();  // signal stopping
    }

    return 0;
  }
};


//==============================================================================
//
Thread::Thread(const char* name)
  : m_pOsTheadData(new OsTheadData(0, name))
{
}

//==============================================================================
//
Thread::Thread(IRunnable& target)
  : m_pOsTheadData(new OsTheadData(&target, "Thread"))
{
}

//==============================================================================
//
Thread::Thread(IRunnable& target, const char* name)
  : m_pOsTheadData(new OsTheadData(&target, name))
{
}

//==============================================================================
//
Thread::~Thread()
{
  if (RUNNING == m_pOsTheadData->m_state)
  {
    stop();
    join();
  }

  delete m_pOsTheadData;
}

//==============================================================================
//
bool Thread::start()
{
  bool success = false;

  if (STOPPED == m_pOsTheadData->m_state)
  {
    pthread_attr_setstacksize(&m_pOsTheadData->m_attr, m_pOsTheadData->m_stacksize);
    if (0 == pthread_create(&m_pOsTheadData->m_tid, &m_pOsTheadData->m_attr, &OsTheadData::threadRun, this))
    {
      m_pOsTheadData->m_sync_sem.wait();
      success = true;
    }
  }

  return success;
}

//==============================================================================
//
void Thread::stop()
{
  m_pOsTheadData->m_state = STOPPED;
}

//==============================================================================
//
bool Thread::isInterrupted()
{
  return m_pOsTheadData->m_state == STOPPED;
}

//==============================================================================
//
bool Thread::join(uint32_t timeout_ms)
{
  if ((RUNNING == m_pOsTheadData->m_state) && (timeout_ms != 0))
  {
    return m_pOsTheadData->m_sync_sem.wait(timeout_ms);
  }

  return true;
}

//==============================================================================
//
void Thread::sleep(uint32_t msec)
{
#if _POSIX_C_SOURCE >= 199309L
  struct timespec ts;
  ts.tv_sec  = msec / 1000;
  ts.tv_nsec = (msec % 1000) * 1000000;
  nanosleep(&ts, NULL);
#else
  usleep(msec * 1000);
#endif
}

//==============================================================================
//
void Thread::yield()
{
  sched_yield();
}

//==============================================================================
//
void Thread::setName(const char* name)
{
  if (m_pOsTheadData->m_state != RUNNING)
  {
    m_pOsTheadData->m_name = name;
  }
}

//==============================================================================
//
const char* Thread::getName()
{
  return m_pOsTheadData->m_name.c_str();
}

//==============================================================================
//
void Thread::setStatcksize(size_t size)
{
  if (m_pOsTheadData->m_state != RUNNING)
  {
    m_pOsTheadData->m_stacksize = size;
  }
}

//==============================================================================
//
size_t Thread::getStacksize()
{
  return m_pOsTheadData->m_stacksize;
}

//==============================================================================
//
void Thread::setPriority(Priority priority)
{
  int newprio;

  switch (priority)
  {
    case INTR_PRIORITY:
      newprio = HIGHEST_THREAD_PRIORITY;
      break;
    case HIGHEST_PRIORITY:
      newprio = HIGH_THREAD_PRIORIY;
      break;
    case HIGH_PRIORITY:
      newprio = MEDIUM_HIGH_THREAD_PRIORITY;
      break;
    case NORMAL_PRIORITY:
      newprio = MEDIUM_THREAD_PRIORITY;
      break;
    case LOW_PRIORITY:
      newprio = LOW_THREAD_PRIORITY;
      break;
    case LOWEST_PRIORITY:
      newprio = LOWEST_THREAD_PRIORITY;
      break;
    default:
      return;
  }

  sched_param param;
  pthread_attr_getschedparam(&m_pOsTheadData->m_attr, &param);
  param.sched_priority = newprio;
  pthread_attr_setschedparam(&m_pOsTheadData->m_attr, &param);
  m_pOsTheadData->m_priority = newprio;
}

//==============================================================================
//
Thread::Priority Thread::getPriority()
{
  Priority priority = NORMAL_PRIORITY;

  switch (m_pOsTheadData->m_priority)
  {
    case HIGHEST_THREAD_PRIORITY:
      priority = INTR_PRIORITY;
      break;
    case HIGH_THREAD_PRIORIY:
      priority = HIGHEST_PRIORITY;
      break;
    case MEDIUM_HIGH_THREAD_PRIORITY:
      priority = HIGH_PRIORITY;
      break;
    case MEDIUM_THREAD_PRIORITY:
      priority = NORMAL_PRIORITY;
      break;
    case LOW_THREAD_PRIORITY:
      priority = LOW_PRIORITY;
      break;
    case LOWEST_THREAD_PRIORITY:
      priority = LOWEST_PRIORITY;
    default:
      break;
  }

  return priority;
}

//==============================================================================
//
void Thread::setTarget(IRunnable& target)
{
  if (m_pOsTheadData->m_state != RUNNING)
  {
    m_pOsTheadData->m_p_target = &target;
  }
}

//==============================================================================
//
Thread::State Thread::getState()
{
  return m_pOsTheadData->m_state;
}

//==============================================================================
//
void Thread::run()
{
  if (m_pOsTheadData->m_p_target != 0)
  {
    m_pOsTheadData->m_p_target->run();
  }
}

} //  namespace os
