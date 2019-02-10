#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SerialPort.h"

//===============================================================================
//
void read(dev::SerialPort& sp)
{
  char read_buffer[100]; 
  int buffer_size   = sizeof(read_buffer);

  int count = sp.Read(read_buffer, buffer_size);

  for (int i = 0; i < count; ++i)
    printf("%c", read_buffer[i]);
  printf("\n");

  fflush(stdout);

  return;
}

//===============================================================================
//
void write(dev::SerialPort& sp)
{
  char write_buffer[100];
  int count;

  count = 0;
  printf("Input Text: ");
  if (fgets(write_buffer, 100, stdin))
    count = strlen(write_buffer);
  // printf("Sending[%d]: %s\n", count, write_buffer);
  sp.Write(write_buffer, count);

  return;
}

//===============================================================================
//
int main(int argc, char* argv[])
{
  dev::SerialPort sp; 
  sp.Open("/dev/ttyACM0", sp.Baud::Baud_115200);

  for (;;)
  {
    write(sp);
    // sleep(5);
    read(sp);
  }

  return 0;
}
