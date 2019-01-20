#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H

#include <unistd.h> 

#include "Thread.h"
#include "Mutex.h"
#include "Semaphore.h"
#include "CircularBuffer.h"

class SerialPortReader : public os::Thread
{
public:
  SerialPortReader(size_t buffer_size);
  bool IsEmpty();
  void Flush();
  int Read(void* buffer, size_t size);
  void Start(int handler);
  void Stop();
  virtual void run();
  ~SerialPortReader();

private:
  int m_handler;
  CircularBuffer m_cbuffer;
  os::Semaphore m_signal;
  os::Mutex m_mutex;
};

#endif
