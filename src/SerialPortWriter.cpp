#include "SerialPortWriter.h"

//===============================================================================
//
SerialPortWriter::SerialPortWriter(size_t buffer_size):
  Thread("SerialPortWriter"),
  m_cbuffer(buffer_size),
  m_handler(-1)
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
int SerialPortWriter::Write(void* buffer, size_t size)
{
  m_mutex.Lock();
  int written = m_cbuffer.Write(buffer, size);
  m_mutex.Unlock();

  m_signal.Post();

  return written;
}  
  
//===============================================================================
//
void SerialPortWriter::Start(int handler)
{
  m_handler = handler;
  Thread::Start();
  return;
}  
  
//===============================================================================
//
void SerialPortWriter::Stop()
{
  Thread::Stop();
  m_signal.Post();
  return;
}  
  
//===============================================================================
//
void SerialPortWriter::Run()
{
  while (!Thread::IsInterrupted())
  {
    m_signal.Wait();

    if (!m_cbuffer.IsEmpty())
    {
      uint8_t data[10];

      m_mutex.Lock();
      size_t size = m_cbuffer.Read(data, sizeof(data));
      m_mutex.Unlock();

      write(m_handler, data, size);
    }
  }

  return;
}  
  
//===============================================================================
//
SerialPortWriter::~SerialPortWriter()
{
  Thread::Stop();
  m_signal.Post();
  return;
}  
  
