CC = g++
CFLAGS = -g -Wall -pthread -std=c++1z
TARGET = isp_determiner_test
MYSQL_CONFIGS := $(shell mysql_config --cflags)
MYSQL_LIBS := $(shell mysql_config --libs)

default: $(TARGET)

$(TARGET): $(TARGET).cxx
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cxx

ussd_test: ussd_test.cxx
	$(CC) $(CFLAGS) -o ussd_test ussd_test.cxx

ussd_program: ussd_program.cxx
	$(CC) $(CFLAGS) -o ussd_program ussd_program.cxx

request_distribution_listener_test: request_distribution_listener_test.cxx
	$(CC) $(CFLAGS) -o request_distribution_listener_test request_distribution_listener_test.cxx

modem_listener_test: modem_listener_test.cxx
	$(CC) $(CFLAGS) -o modem_listener_test modem_listener_test.cxx

modem_test: modem_test.cxx
	$(CC) $(CFLAGS) -o modem_test modem_test.cxx $(MYSQL_CONFIGS) $(MYSQL_LIBS)

modem: modem_test.cxx
	$(CC) $(CFLAGS) -c -o modem.o modem.cxx $(MYSQL_CONFIGS) $(MYSQL_LIBS)
