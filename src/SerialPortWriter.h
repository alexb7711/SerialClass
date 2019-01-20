#ifndef SERIALPORTWRITER_H
#define SERIALPORTWRITER_H

#include <unistd.h> 

#include "Thread.h"
#include "Mutex.h"
#include "Semaphore.h"
#include "CircularBuffer.h"

class SerialPortWriter : public os::Thread
{
public:
  SerialPortWriter(size_t buffer_size);
  bool IsFull();
  void Flush();
  int Write(const void* buffer, size_t size);
  void Start(int handler);
  void Stop();
  virtual void run();
  ~SerialPortWriter();

private:
  int m_handler; 
  CircularBuffer m_cbuffer;
  os::Semaphore m_signal;
  os::Mutex m_mutex;

};

#endif
