///
/// @file os/posix/Mutex.cpp
///
/// This file contains the Mutex class posix implementation.
///

#include <pthread.h>
#include <Mutex.h>

namespace os
{

struct Mutex::OsMutexData
{
  pthread_mutex_t     mutex;
  pthread_mutexattr_t attr;
};

//==============================================================================
//
Mutex::Mutex()
  : m_pOsMutexData(new OsMutexData)
{
  pthread_mutex_init(&m_pOsMutexData->mutex, &m_pOsMutexData->attr);
}

//==============================================================================
//
Mutex::~Mutex()
{
  pthread_mutex_destroy(&m_pOsMutexData->mutex);
  delete m_pOsMutexData;
}

//==============================================================================
//
void Mutex::lock()
{
  pthread_mutex_lock(&m_pOsMutexData->mutex);
}

//==============================================================================
//
bool Mutex::tryLock()
{
  return pthread_mutex_trylock(&m_pOsMutexData->mutex) == 0;
}

//==============================================================================
//
void Mutex::unlock()
{
  pthread_mutex_unlock(&m_pOsMutexData->mutex);
}

} // end os
