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

  for (;;)
  {
    int count = sp.Read(read_buffer, buffer_size);

    for (int i = 0; i < count; ++i)
      printf("%c", read_buffer[i]);

    fflush(stdout);
  }

  return;
}

//===============================================================================
//
void write(dev::SerialPort& sp)
{
  char write_buffer[100];
  int count;

  for (;;)
  {
    count = 0;
    printf("Input Text: ");
    if (fgets(write_buffer, 100, stdin))
      count = strlen(write_buffer);
    printf("Sending[%d] '%s'\n", count, write_buffer);
    sp.Write(write_buffer, count);
  }

  return;
}

//===============================================================================
//
int main(int argc, char* argv[])
{
  dev::SerialPort sp; 

  if(argc == 1)
  {
    printf("Reading...\n");
    sp.Open("/dev/ttyUSB0", sp.Baud::Baud_38400);
    read(sp);
  }
  else
  {
    printf("Writing...\n");
    sp.Open("/dev/ttyUSB1", sp.Baud::Baud_38400);
    write(sp);
  }

  return 0;
}
