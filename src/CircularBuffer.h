#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

class CircularBuffer 
{
public:
  CircularBuffer(size_t capacity);
  ~CircularBuffer();

  size_t Capacity();
  size_t Size();

  bool IsEmpty();
  bool IsFull();
  void Clear();

  int Write(const void* buffer, size_t count);
  int Read(void* buffer, size_t count);

private:
  size_t         m_capacity;
  size_t         m_size;
  unsigned char* m_buffer;
  size_t         m_head;
  size_t         m_tail;
};

#endif // CIRCULARBUFFER_H
