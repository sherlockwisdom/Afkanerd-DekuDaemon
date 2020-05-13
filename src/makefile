CC = g++
CFLAGS = -g -Wall -pthread -std=c++1z
# CFLAGS = -g -Wl,--start-group -pthread -std=c++1z
TARGET = start
MYSQL_CONFIGS := $(shell mysql_config --cflags)
MYSQL_LIBS := $(shell mysql_config --libs)
LOBJECTS := build/logger.o build/helpers.o build/sys_calls.o build/ussd.o build/mysql.o build/request_distribution_listener.o  build/modem_listener.o build/start.o build/modem.o
COBJECTS := sub-routines/ussd.cxx formatters/logger.cxx formatters/helpers.cxx sys_calls/sys_calls.cxx mysql/mysql.cxx sub-routines/modem.cxx sub-routines/request_distribution_listener.cxx sub-routines/modem_listener.cxx start.cxx

# Compile only those files that have changed and not the entire thing
# Keep the object files in a dir, to avoid recompiling all the time


all:
	$(CC) $(CFLAGS) -c sub-routines/ussd.cxx -o build/ussd.o
	$(CC) $(CFLAGS) -c formatters/logger.cxx -o build/logger.o 
	$(CC) $(CFLAGS) -c formatters/helpers.cxx -o build/helpers.o 
	$(CC) $(CFLAGS) -c sys_calls/sys_calls.cxx -o build/sys_calls.o
	$(CC) $(CFLAGS) -c mysql/mysql.cxx -o build/mysql.o $(MYSQL_LIBS) $(MYSQL_CONFIGS)
	$(CC) $(CFLAGS) -c sub-routines/modem.cxx -o build/modem.o $(MYSQL_CONFIGS) $(MYSQL_LIBS)
	$(CC) $(CFLAGS) -c sub-routines/request_distribution_listener.cxx -o build/request_distribution_listener.o 
	$(CC) $(CFLAGS) -c sub-routines/modem_listener.cxx -o build/modem_listener.o $(MYSQL_CONFIGS) $(MYSQL_LIBS)
	$(CC) $(CFLAGS) -c start.cxx -o build/start.o $(MYSQL_CONFIGS) $(MYSQL_LIBS)
	$(CC) $(CFLAGS) $(LOBJECTS)  -o bin/start $(MYSQL_CONFIGS) $(MYSQL_LIBS)
	
# Cleanup begins from here
	# rm $(LOBJECTS)

