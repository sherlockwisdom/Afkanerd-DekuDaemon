CC = g++
CFLAGS = -g -Wall -pthread -std=c++1z -I$(CPPUTEST_HOME)/include
MYSQL_CONFIGS := $(shell mysql_config --cflags)
MYSQL_LIBS := $(shell mysql_config --libs)
DEFAULT_DEPENDENCIES := ../src/mysql/mysql.cxx ../src/formatters/logger.cxx

ussd_test: ussd_test.cxx
	$(CC) $(CFLAGS) -o $@.o $@.cxx -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt $(MYSQL_CONFIGS) $(MYSQL_LIBS)
mysql_test: mysql_test.cxx
	$(CC) $(CFLAGS) -o $@.o $@.cxx $(DEFAULT_DEPENDENCIES) -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt $(MYSQL_CONFIGS) $(MYSQL_LIBS)

modem_test: modem_test.cxx
	$(CC) $(CFLAGS) -o $@.o $@.cxx ../src/sub-routines/modem.cxx ../src/sub-routines/ussd.cxx ../src/formatters/helpers.cxx ../src/sys_calls/sys_calls.cxx ../src/sub-routines/request_distribution_listener.cxx $(DEFAULT_DEPENDENCIES) -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt $(MYSQL_CONFIGS) $(MYSQL_LIBS)

sys_calls_test: sys_calls_test.cxx
	$(CC) $(CFLAGS) -o $@.o $@.cxx -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt $(MYSQL_CONFIGS) $(MYSQL_LIBS)
