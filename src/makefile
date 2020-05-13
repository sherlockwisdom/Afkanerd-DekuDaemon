CC = g++
CFLAGS = -g -Wall -pthread -std=c++1z
# CFLAGS = -g -Wl,--start-group -pthread -std=c++1z
TARGET = start
MYSQL_CONFIGS := $(shell mysql_config --cflags)
MYSQL_LIBS := $(shell mysql_config --libs)
LOBJECTS := ../builds/logger.o ../builds/helpers.o ../builds/sys_calls.o ../builds/ussd.o ../builds/mysql.o ../builds/request_distribution_listener.o  ../builds/modem_listener.o ../builds/start.o ../builds/modem.o
COBJECTS := sub-routines/ussd.cxx formatters/logger.cxx formatters/helpers.cxx sys_calls/sys_calls.cxx mysql/mysql.cxx sub-routines/modem.cxx sub-routines/request_distribution_listener.cxx sub-routines/modem_listener.cxx start.cxx

# Compile only those files that have changed and not the entire thing
# Keep the object files in a dir, to avoid recompiling all the time


all: $(COBJECTS)
	$(CC) $(CFLAGS) -c sub-routines/ussd.cxx -o ../builds/ussd.o
	$(CC) $(CFLAGS) -c formatters/logger.cxx -o ../builds/logger.o 
	$(CC) $(CFLAGS) -c formatters/helpers.cxx -o ../builds/helpers.o 
	$(CC) $(CFLAGS) -c sys_calls/sys_calls.cxx -o ../builds/sys_calls.o
	$(CC) $(CFLAGS) -c mysql/mysql.cxx -o ../builds/mysql.o $(MYSQL_LIBS) $(MYSQL_CONFIGS)
	$(CC) $(CFLAGS) -c sub-routines/modem.cxx -o ../builds/modem.o $(MYSQL_CONFIGS) $(MYSQL_LIBS)
	$(CC) $(CFLAGS) -c sub-routines/request_distribution_listener.cxx -o ../builds/request_distribution_listener.o 
	$(CC) $(CFLAGS) -c sub-routines/modem_listener.cxx -o ../builds/modem_listener.o $(MYSQL_CONFIGS) $(MYSQL_LIBS)
	$(CC) $(CFLAGS) -c start.cxx -o ../builds/start.o $(MYSQL_CONFIGS) $(MYSQL_LIBS)
	$(CC) $(CFLAGS) $(LOBJECTS)  -o ../bins/start $(MYSQL_CONFIGS) $(MYSQL_LIBS)
	
# Cleanup begins from here
	# rm $(LOBJECTS)

