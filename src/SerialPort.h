#ifndef DEV_SERIALPORT_H
#define DEV_SERIALPORT_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <fcntl.h> // Remove after SerialPortReader is implemented!!!
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include "SerialPortWriter.h"

namespace dev 
{

/// class description.
class SerialPort
{
public:
  /// Default object constructor.
  SerialPort();

  /// Object destructor.
  virtual ~SerialPort();

  enum Baud
  {
    Baud_110 = 1,
    Baud_300, 
    Baud_600, 
    Baud_1200,
    Baud_2400, 
    Baud_4800,
    Baud_9600,
    Baud_19200,
    Baud_38400,
    Baud_57600,
    Baud_115200,
  };

  
  bool Open(const char* dev, Baud baud);
  void Close();
  void Flush();
  int Read(void* buffer, size_t size, bool block=true);
  int Write(const void* buffer, size_t size);
  bool IsEmpty();
  bool IsFull();

private:
  SerialPortWriter m_writer;
  int m_serialPort;
  int m_baudRate;

  int ConvBaud(Baud baud);
  bool Configure(Baud baud);
};

} // end namespace dev

#endif // DEV_SERIALPORT_H

