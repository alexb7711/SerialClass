all: main.o SerialPort.o
	g++ -o main main.o SerialPort.o

main.o: main.cpp
	g++ -c main.cpp

SerialPort.o: SerialPort.cpp
	g++ -c SerialPort.cpp
