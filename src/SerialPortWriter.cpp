#include <stdio.h>
#include "SerialPortWriter.h"

//===============================================================================
//
SerialPortWriter::SerialPortWriter(size_t buffer_size):
  Thread("SerialPortWriter"),
  m_handler(-1),
  m_cbuffer(buffer_size)
{}  
  
//===============================================================================
//
bool SerialPortWriter::IsFull()
{
  return m_cbuffer.IsFull();
}  
  
//===============================================================================
//
void SerialPortWriter::Flush()
{}  
  
//===============================================================================
//
int SerialPortWriter::Write(const void* buffer, size_t size)
{
  m_mutex.lock();
  int written = m_cbuffer.Write(buffer, size);
  m_mutex.unlock();

  printf("Posting...\n");
  m_signal.post();

  return written;
}  
  
//===============================================================================
//
void SerialPortWriter::Start(int handler)
{
  m_handler = handler;
  Thread::start();
  return;
}  
  
//===============================================================================
//
void SerialPortWriter::Stop()
{
  Thread::stop();
  m_signal.post();
  return;
}  
  
//===============================================================================
//
void SerialPortWriter::run()
{
  while (!Thread::isInterrupted())
  {
    printf("Waiting...\n");
    m_signal.wait();
    printf("Running!!!\n");

    printf("Signal Detected...\n");

    if (!m_cbuffer.IsEmpty())
    {
      uint8_t data[10];

      m_mutex.lock();
      size_t size = m_cbuffer.Read(data, sizeof(data));
      
      for (size_t i = 0; i < size; ++i)
        printf("%c", data[i]);
      m_mutex.unlock();

      write(m_handler, data, size);
    }
  }

  return;
}  
  
//===============================================================================
//
SerialPortWriter::~SerialPortWriter()
{
  Thread::stop();
  m_signal.post();
  return;
}  
  
