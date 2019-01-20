/// @file    lib/interface/IRunnable.h
///
/// Base class for runnable classes.
///

#ifndef IRUNNABLE_H
#define IRUNNABLE_H

namespace interface
{

/// The IRunnable interface should be implemented by any class whose instances
/// are
/// intended to be executed by a thread.
class IRunnable
{
public:
  // Base virtual destructor
  virtual ~IRunnable() {}

  /// method to be called in that separately executing thread.
  virtual void run() = 0;
};

} // end interface

#endif // IRUNNABLE_H
