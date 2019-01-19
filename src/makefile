CC     = g++
CLFAGS = -Wall
OBJDIR = obj
TARGET = main
SRC    = main.cpp SerialPort.cpp
OBJ    = $(SRC:%.cpp=%.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	cd $(OBJDIR) && $(CC) $(CLFAGS) $(OBJ) -o ../$@ 

.cpp.o: $(SRC)
	$(CC) $(CLFAGS) -c $< -o $(OBJDIR)/$@ 

clean:
	$(RM) $(OBJ) 
