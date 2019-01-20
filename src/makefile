CC     = g++
CLFAGS = -Wall
OBJDIR = obj
TARGET = main
SRC    = main.cpp SerialPort.cpp CircularBuffer.cpp SerialPortWriter.cpp SerialPortReader.cpp Thread.cpp Mutex.cpp Semaphore.cpp
OBJ    = $(SRC:%.cpp=%.o)
INCLUDES = -I.

all: $(TARGET)

$(TARGET): $(OBJ)
	cd $(OBJDIR) && $(CC) $(CLFAGS) $(OBJ) -o ../$@ -lpthread -lrt

.cpp.o: $(SRC)
	$(CC) $(CLFAGS) $(INCLUDES) -c $< -o $(OBJDIR)/$@ -lpthread -lrt

clean: 
	rm -rf obj/
	mkdir obj/
