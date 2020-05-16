CC = g++
CFLAGS = -g -Wall -pthread -std=c++1z
# CFLAGS = -g -Wl,--start-group -pthread -std=c++1z
MYSQL_CONFIGS := $(shell mysql_config --cflags)
MYSQL_LIBS := $(shell mysql_config --libs)

mysql : mysql.cxx
	$(CC) $(CFLAGS) -c mysql.cxx -o mysql.o $(MYSQL_LIBS) $(MYSQL_CONFIGS)
