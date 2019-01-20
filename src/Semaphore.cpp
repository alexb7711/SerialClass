///
/// @file os/linux/Semaphore.cpp
///
/// This file contains the Semaphore class posix implementation.
///

#include <semaphore.h>
#include <unistd.h>
#include <Semaphore.h>

namespace os
{

struct Semaphore::OsSemaphoreData
{
  sem_t semaphore;
};

//==============================================================================
//
Semaphore::Semaphore(unsigned int initval)
  : m_pOsSemaphoreData(new OsSemaphoreData)
{
  sem_init(&m_pOsSemaphoreData->semaphore, 0, initval);
}

//==============================================================================
//
Semaphore::~Semaphore()
{
  sem_destroy(&m_pOsSemaphoreData->semaphore);
  delete m_pOsSemaphoreData;
}

//==============================================================================
//
bool Semaphore::post()
{
  return sem_post(&m_pOsSemaphoreData->semaphore) == 0;
}

//==============================================================================
//
bool Semaphore::wait(unsigned int timeout_ms)
{
  bool success = true;

  if (WAIT_FOREVER == timeout_ms)
  {
    success = sem_wait(&m_pOsSemaphoreData->semaphore) == 0;
  }
  else
  {
    struct timespec ts;

    ts.tv_sec  = timeout_ms / 1000;
    ts.tv_nsec = (timeout_ms % 1000) * 1000000;
    success    = sem_timedwait(&m_pOsSemaphoreData->semaphore, &ts) == 0;
  }

  return success;
}

} // end os
