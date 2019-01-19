#include <fcntl.h> 

#include "Thread.h"
#include "Mutix.h"
#include "Semaphore.h"
#include "CircularBuffer.h"

class SerialPortWriter : public Thread
{
public:
  SerialPortWriter(size_t buffer_size);
  bool IsFull();
  void Flush();
  int Write(void* buffer, size_t size);
  void Start(int handler);
  void Stop();
  virtual void Run();
  ~SerialPortWriter();

private:
  int m_handler; 
  CircularBuffer m_cbuffer;
  Semaphore m_signal;
  Mutex m_mutex;

};
