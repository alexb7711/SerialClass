#include <stdio.h>

#include "SerialPortReader.h"

//===============================================================================
//
SerialPortReader::SerialPortReader(size_t buffer_size):
  Thread("SerialPortReader"),
  m_handler(0),
  m_cbuffer(buffer_size)
{}

//===============================================================================
//
bool SerialPortReader::IsEmpty()
{
  return m_cbuffer.IsEmpty();
}

//===============================================================================
//
void SerialPortReader::Flush()
{}

//===============================================================================
//
int SerialPortReader::Read(void* buffer, size_t size)
{
  // printf("Waiting...\n");
  m_signal.wait();
  // printf("Running...\n");

  m_mutex.lock();
  int read = m_cbuffer.Read(buffer, size);
  m_mutex.unlock();  

  return read;
}

//===============================================================================
//
void SerialPortReader::Start(int handler)
{
  m_handler = handler;
  Thread::start();
  return;
}

//===============================================================================
//
void SerialPortReader::Stop()
{
  Thread::stop();
  m_signal.post();
  return;
}

//===============================================================================
//
void SerialPortReader::run()
{
  while (!Thread::isInterrupted())
  {
    uint8_t data[1];

    int size = read(m_handler, data, sizeof(data));

    m_mutex.lock();
    m_cbuffer.Write(data, size);
    m_mutex.unlock();
    
    // printf("Posting...");
    m_signal.post();
  }

  return;
}
  
//===============================================================================
//
SerialPortReader::~SerialPortReader()
{
  Thread::stop();
  m_signal.post();
  return;
}
