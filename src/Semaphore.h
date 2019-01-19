///
/// @file lib/os/Semaphore.hpp
///
/// This file contains the Semaphore class definition.
///

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <lib/os/Platform.h>

namespace os
{

class Semaphore
{
public:
  enum Timeout { WAIT_FOREVER = 0xFFFFFFFFU };

  /// Constructor
  ///
  /// @param[in] initval    Initial value
  Semaphore(uint32_t initval=0);

  /// Destructor
  ~Semaphore();

  /// Increment the value of the semaphor
  ///
  /// @return true if the calling process successfully performed
  bool post();

  /// Wait on the semaphore with timeout
  ///
  /// @param[in] timeout_ms  Time to wait in milliseconds
  ///
  /// @return true if the calling process successfully performed
  bool wait(uint32_t timeout_ms=WAIT_FOREVER);

private:
  /// Disallow the copy constructor.
  Semaphore(const Semaphore& other);

  /// Disallow the assignment operator.
  Semaphore& operator=(const Semaphore& other);

  struct OsSemaphoreData;
  OsSemaphoreData* m_pOsSemaphoreData; ///< OS specific data.
};

} // end os

#endif // SEMAPHORE_H
