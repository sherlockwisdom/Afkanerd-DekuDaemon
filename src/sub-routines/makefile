CC = g++
CFLAGS = -g -Wall -pthread -std=c++1z
TARGET = modem
MYSQL_CONFIGS := $(shell mysql_config --cflags)
MYSQL_LIBS := $(shell mysql_config --libs)

default: $(TARGET)

$(TARGET): $(TARGET).cxx
	$(CC) $(CFLAGS) -c -o $(TARGET).o $(TARGET).cxx $(MYSQL_CONFIGS) $(MYSQL_LIBS)

ussd_program: ussd_program.cxx
	$(CC) $(CFLAGS) -o ussd_program ussd_program.cxx

modem_listener: modem_listener.cxx
	$(CC) $(CFLAGS) -c -o $@.o $@.cxx $(MYSQL_CONFIGS) $(MYSQL_LIBS)

sms_messages: modem_sms_test.cxx
	$(CC) $(CFLAGS) -o sms_messages.o modem_sms_test.cxx $(MYSQL_CONFIGS) $(MYSQL_LIBS)

