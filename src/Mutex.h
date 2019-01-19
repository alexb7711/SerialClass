///
/// @file lib/os/Mutex.h
///
/// This file contains the Mutex class definition.
///

#ifndef MUTEX_H
#define MUTEX_H

namespace os
{

class Mutex
{
public:
  /// Default object constructor.
  Mutex();

  /// Object destructor.
  virtual ~Mutex();

  /// locks the mutex, blocks if the mutex is not available
  void lock();

  /// tries to lock the mutex, returns if the mutex is not available
  ///
  /// return true if the lock was acquired successfully, otherwise false
  bool tryLock();

  /// unlocks the mutex
  void unlock();

private:
  /// Disallow the copy constructor.
  Mutex(const Mutex& other);

  /// Disallow the assignment operator.
  Mutex& operator=(const Mutex& other);

  struct OsMutexData;
  OsMutexData* m_pOsMutexData; ///< OS specific data
};

} // end os

#endif // MUTEX_H
