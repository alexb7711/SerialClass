/// @file    lib/os/Thread.h
///
/// Contains the Thread class definition.
///

#ifndef THREAD_H
#define THREAD_H

#include <Platform.h>
#include <IRunnable.h>

namespace os
{

/// Thread wrapper class used for simplifying thread control
class Thread : public interface::IRunnable
{
  typedef interface::IRunnable IRunnable;

public:
  enum Timeout { WAIT_FOREVER = 0xFFFFFFFFU };

  enum State
  {
    STOPPED,   ///< Flag for stopped thread
    RUNNING    ///< Flag for running thread
  };

  enum Priority
  {
    LOWEST_PRIORITY       = 0,
    LOW_PRIORITY          = 2,
    NORMAL_PRIORITY       = 4,
    ABOVE_NORMAL_PRIORITY = 5,
    HIGH_PRIORITY         = 6,
    HIGHEST_PRIORITY      = 8,
    INTR_PRIORITY         = 10
  };

  /// Constructor with name
  /// @param[in] name -- Optional thread name
  Thread(const char* name);

  /// Constructor with target
  /// @param[in] target -- IRunnable object
  Thread(IRunnable& target);

  /// Constructor with name and target
  /// @param[in] target -- IRunnable object
  /// @param[in] name -- Optional thread name
  Thread(IRunnable& target, const char* name);

  /// Object destructor
  ///
  virtual ~Thread();

  /// Retrieve thread name
  /// @return threads name
  const char* getName();

  /// Retrieve threads priority
  /// @return Priority
  Priority getPriority();

  /// Retrieve threads stack size
  /// @return stack size
  size_t getStacksize();

  /// retrieve threads state
  /// @return State
  State getState();

  /// Set threads name.
  /// The name will not be updated if the thread is already running.
  /// @param[in] name -- Threads name
  void setName(const char* name);

  /// Set the threads priority.
  /// This function has no effect if the thread is already running.
  /// @param[in] priority -- Threads priority
  void setPriority(Priority priority);

  /// Set the threads stack size.
  /// This function has no effect if the thread is already running.
  /// @param[in] size -- Threads stack size
  void setStatcksize(size_t size);

  /// Set threads runnable object.
  /// This function has no effect if the thread is already running.
  /// @param[in] target -- IRunnable object
  void setTarget(IRunnable& target);

  /// Start the threads.
  virtual bool start();

  /// Interrupts the thread.
  virtual void stop();
  // virtual void interrupt();

  /// Retrieved the state of the interrupted flag
  virtual bool isInterrupted();

  /// Waits for the thread to stop execution
  /// @param[in] timeout_ms -- Time to wait for the thread to stop
  /// @return true if thread joined successfuly
  bool join(uint32_t timeout_ms=WAIT_FOREVER);

  /// Global function to put the current thread to sleep.
  /// @para[in] msec -- Number of milliseconds for the thread to sleep
  static void sleep(uint32_t msec);

  /// Global function to have the current thread yield the CPU.
  static void yield();

protected:
  /// Main thread method
  virtual void run();

private:
  /// Disallow the copy constructor.
  Thread(const Thread&);

  /// Disallow the assignment operator.
  Thread& operator=(const Thread&);

  struct OsTheadData;
  OsTheadData* m_pOsTheadData; ///< OS specific data.
};

} // end os

#endif // THREAD_H
