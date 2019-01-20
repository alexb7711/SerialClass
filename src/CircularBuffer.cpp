#include <cstring>
#include "CircularBuffer.h"

//=====================================================================================================================
//
CircularBuffer::CircularBuffer(size_t capacity) 
  : m_capacity(capacity)
  , m_size(0)
  , m_buffer(new unsigned char[capacity])
  , m_head(0)
  , m_tail(0)
{}

//=====================================================================================================================
//
CircularBuffer::~CircularBuffer() 
{
  delete[] m_buffer;
}

//=====================================================================================================================
//
size_t CircularBuffer::Capacity() 
{
  return m_capacity;
}

//=====================================================================================================================
//
size_t CircularBuffer::Size() 
{
  return m_size;
}

//=====================================================================================================================
//
void CircularBuffer::Clear() 
{
  m_head = m_tail = 0;
  m_size = 0;
}

//=====================================================================================================================
//
bool CircularBuffer::IsEmpty() 
{
  return m_size == 0;
}

//=====================================================================================================================
//
bool CircularBuffer::IsFull() 
{
  return m_size == m_capacity;
}

//=====================================================================================================================
//
int CircularBuffer::Write(const void* buffer, size_t count) 
{
  const unsigned char* buff = static_cast<const unsigned char*>(buffer);
  size_t available          = m_capacity - m_size;
  size_t written            = 0;

  while ((available > 0) && (count > 0))
  {
    size_t nbytes = (m_head >= m_tail) ? m_capacity - m_head : m_tail - m_head;

    if (nbytes > count) nbytes = count;

    memcpy(&m_buffer[m_head], buff, nbytes);

    buff      += nbytes;
    written   += nbytes;
    count     -= nbytes;
    available -= nbytes;
    m_size    += nbytes;
    m_head    += nbytes;

    if (m_head >= m_capacity) m_head = 0;
  }

  return written;
}

//=====================================================================================================================
//
int CircularBuffer::Read(void* buffer, size_t count) 
{
  unsigned char* buff = static_cast<unsigned char*>(buffer);
  size_t available          = m_size;
  size_t read               = 0;

  while ((available > 0) && (count > 0))
  {
    size_t nbytes = (m_head > m_tail) ? m_head - m_tail : m_capacity - m_tail;

    if (nbytes > count) nbytes = count;

    memcpy(buff, &m_buffer[m_tail], nbytes);

    buff      += nbytes;
    read      += nbytes;
    count     -= nbytes;
    available -= nbytes;
    m_size    -= nbytes;
    m_tail    += nbytes;

    if (m_tail >= m_capacity) m_tail = 0;
  }

  return read;
}
