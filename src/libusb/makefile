CC = g++
CFLAGS = -g -Wall -pthread -std=c++1z `pkg-config --libs --cflags libusb-1.0`
TARGET = main
MYSQL_CONFIGS := $(shell mysql_config --cflags)
MYSQL_LIBS := $(shell mysql_config --libs)

default: $(TARGET)

$(TARGET): $(TARGET).cxx
	$(CC) -o $(TARGET).o $(TARGET).cxx ../formatters/loggers.cxx -o main $(CFLAGS)
