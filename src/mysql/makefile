CC = g++
CFLAGS = -g -Wall -pthread -std=c++1z
MYSQL_CONFIGS := $(shell mysql_config --cflags)
MYSQL_LIBS := $(shell mysql_config --libs)
TARGET = main

default: $(TARGET)

$(TARGET): $(TARGET).cxx
	$(CC) $(CFLAGS) $(MYSQL_CONFIGS) $(MYSQL_LIBS) -o $(TARGET) $(TARGET).cxx

