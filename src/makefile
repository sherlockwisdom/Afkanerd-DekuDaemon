CC = g++
CFLAGS = -g -Wall -pthread -std=c++1z
# CFLAGS = -g -Wl,--start-group -pthread -std=c++1z
TARGET = start
MYSQL_CONFIGS := $(shell mysql_config --cflags)
MYSQL_LIBS := $(shell mysql_config --libs)
LOBJECTS := logger.o helpers.o sys_calls.o ussd.o mysql.o request_distribution_listener.o modem.o  modem_listener.o start.o

# Compile only those files that have changed and not the entire thing
# Keep the object files in a dir, to avoid recompiling all the time


all:
	$(CC) $(CFLAGS) -c sub-routines/ussd.cxx -o ussd.o
	$(CC) $(CFLAGS) -c formatters/logger.cxx -o logger.o 
	$(CC) $(CFLAGS) -c formatters/helpers.cxx -o helpers.o 
	$(CC) $(CFLAGS) -c sys_calls/sys_calls.cxx -o sys_calls.o
	$(CC) $(CFLAGS) -c mysql/mysql.cxx -o mysql.o $(MYSQL_LIBS) $(MYSQL_CONFIGS)
	$(CC) $(CFLAGS) -c sub-routines/modem.cxx -o modem.o $(MYSQL_CONFIGS) $(MYSQL_LIBS)
	$(CC) $(CFLAGS) -c sub-routines/request_distribution_listener.cxx -o request_distribution_listener.o 
	$(CC) $(CFLAGS) -c sub-routines/modem_listener.cxx -o modem_listener.o $(MYSQL_CONFIGS) $(MYSQL_LIBS)
	$(CC) $(CFLAGS) -c start.cxx -o start.o $(MYSQL_CONFIGS) $(MYSQL_LIBS)
	$(CC) $(CFLAGS) $(LOBJECTS) -o start $(MYSQL_CONFIGS) $(MYSQL_LIBS)


start:
	$(CC) $(CFLAGS) -c start.cxx -o start.o $(MYSQL_CONFIGS) $(MYSQL_LIBS)
