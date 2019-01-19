///
/// @file Cps/lib/os/posix/Semaphore.cpp
///
/// This file contains the Semaphore class posix implementation.
///

#include <semaphore.h>
#include <unistd.h>
#include <Cps/lib/os/sleep_ms.h>
#include <Cps/lib/os/Semaphore.hpp>

namespace cps
{
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
  sem_init(&m_pOsSemaphoreData->semaphore, 1, initval);
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

  if (timeout_ms == constant::WAIT_FOREVER)
  {
    success = Semaphore::wait();
  }
  else
  {
    while (sem_trywait(&m_pOsSemaphoreData->semaphore) != 0)
    {
      if (timeout_ms < 2)
      {
        success = false;
        break;
      }

      timeout_ms--;
      sleep_ms(1);
    }
  }

  return success;
}

} // end os
} // end cps
