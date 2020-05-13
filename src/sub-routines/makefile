CC = g++
CFLAGS = -g -Wall -pthread -std=c++1z
MYSQL_CONFIGS := $(shell mysql_config --cflags)
MYSQL_LIBS := $(shell mysql_config --libs)
REQUIRED_FILES := request_distribution_listener.cxx

modem: modem.cxx
	$(CC) $(CFLAGS) -c -o $@.o $@.cxx $(MYSQL_CONFIGS) $(MYSQL_LIBS)

ussd: ussd.cxx
	$(CC) $(CFLAGS) -c -o $@.o $@.cxx $(MYSQL_CONFIGS) $(MYSQL_LIBS)

modem_listener: modem_listener.cxx
	$(CC) $(CFLAGS) -c -o $@.o $@.cxx $(MYSQL_CONFIGS) $(MYSQL_LIBS)

sms_messages: modem_sms_test.cxx
	$(CC) $(CFLAGS) -o sms_messages.o modem_sms_test.cxx $(MYSQL_CONFIGS) $(MYSQL_LIBS)

