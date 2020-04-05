CC = g++
CFLAGS = -g -Wall -pthread -std=c++1z -I$(CPPUTEST_HOME)/include
TARGET = complete_system_test
MYSQL_CONFIGS := $(shell mysql_config --cflags)
MYSQL_LIBS := $(shell mysql_config --libs)

default: $(TARGET)

$(TARGET): $(TARGET).cxx
	$(CC) $(CFLAGS) -o $(TARGET).o $(TARGET).cxx -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt

