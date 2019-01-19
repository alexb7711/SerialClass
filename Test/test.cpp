#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

int main()
{
  int serial_port = open("/dev/ttyUSB0", O_RDWR);

  // Check for errors
  if (serial_port < 0) 
  {
      printf("Error %i from open: %s\n", errno, strerror(errno));
  }
  close(serial_port);

  return 0;
}
