#include "SerialPort.h"

namespace dev
{

//========//
// Public //
//========//

//===============================================================================
//
SerialPort::SerialPort():
  m_serialPort(0),
  m_baudRate(0)
{}

//===============================================================================
//
int SerialPort::Open(const char* dev, Baud baud)
{
  int portOpened = 1; 
  m_serialPort = open(dev, O_RDWR);

  struct termios tty;
  memset(&tty, 0, sizeof(tty));

  if (tcgetattr(m_serialPort, &tty) != 1)
  {
    portOpened = 0; 
    printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
  }
  
  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  tty.c_cflag |= CS8; // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE; // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

  m_baudRate = ConvBaud(baud);
  cfsetispeed(&tty, m_baudRate);
  cfsetospeed(&tty, m_baudRate);

  if (tcsetattr(m_serialPort, TCSANOW, &tty) != 0) 
  {
    portOpened = 0; 
    printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
  }

  return portOpened;
}

//===============================================================================
//
SerialPort::~SerialPort()
{}

//===============================================================================
//
void SerialPort::Close()
{
  close(m_serialPort);
  return;
}

//===============================================================================
//
void SerialPort::Flush()
{}

//===============================================================================
//
int SerialPort::Read(void* buffer, size_t size, bool block)
{
  int bitsRead = read(m_serialPort, buffer, size); 
  return bitsRead;
}

//===============================================================================
//
int SerialPort::Write(const void* buffer, size_t size)
{
  int bitsWritten = write(m_serialPort, buffer, size);
  return bitsWritten;
}

//===============================================================================
//
bool SerialPort::IsEmpty()
{
  return 0;
}

//===============================================================================
//
bool SerialPort::IsFull()
{
  return 0;
}

//=========//
// Private //
//=========//

//===============================================================================
//
int SerialPort::ConvBaud(Baud baud)
{
  switch(baud)
  {
    default          : return -1;
    case Baud_110    : return B110;
    case Baud_300    : return B300;
    case Baud_600    : return B600;
    case Baud_1200   : return B1200;
    case Baud_2400   : return B2400;
    case Baud_4800   : return B4800;
    case Baud_9600   : return B9600;
    case Baud_19200  : return B19200;
    case Baud_38400  : return B38400;
    case Baud_57600  : return B57600;
    case Baud_115200 : return B115200;
  }
}

} // end dev

