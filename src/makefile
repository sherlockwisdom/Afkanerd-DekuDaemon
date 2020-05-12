CC = g++
CFLAGS = -g -Wall -pthread -std=c++1z
TARGET = start
MYSQL_CONFIGS := $(shell mysql_config --cflags)
MYSQL_LIBS := $(shell mysql_config --libs)
REQUIRED_FILES := sub-routines/modem_listener.cxx sub-routines/modem.cxx sub-routines/request_distribution_listener.cxx 


all:
	$(CC) $(CFLAGS) sub-routines/modem.cxx -o modem.o $(MYSQL_CONFIGS) $(MYSQL_LIBS)
	$(CC) $(CFLAGS) sub-routines/modem_listener.cxx -o modem_listener.o $(MYSQL_CONFIGS) $(MYSQL_LIBS)
	$(CC) $(CFLAGS) sub-routines/request_distribution_listener.cxx -o request_distribution_listener.o $(MYSQL_CONFIGS) $(MYSQL_LIBS)
	$(CC) $(CFLAGS) formatters/helpers.cxx -o helpers.o $(MYSQL_CONFIGS) $(MYSQL_LIBS)
	$(CC) $(CFLAGS) formatters/loggers.cxx -o loggers.o $(MYSQL_CONFIGS) $(MYSQL_LIBS)
