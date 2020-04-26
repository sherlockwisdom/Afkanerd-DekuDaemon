CC = g++
CFLAGS = -g -Wall -pthread -std=c++1z -I$(CPPUTEST_HOME)/include
MYSQL_CONFIGS := $(shell mysql_config --cflags)
MYSQL_LIBS := $(shell mysql_config --libs)

parsers: parsers_test.cxx
	$(CC) $(CFLAGS) -o parsers_test.o parsers_test.cxx -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt

mysql_test: mysql_test.cxx
	$(CC) $(CFLAGS) -o $@.o $@.cxx -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt $(MYSQL_CONFIGS) $(MYSQL_LIBS)
