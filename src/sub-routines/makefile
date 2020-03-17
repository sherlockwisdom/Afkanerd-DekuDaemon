CC = g++
CFLAGS = -g -Wall -pthread -std=c++1z
TARGET = isp_determiner_test

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
	$(CC) $(CFLAGS) -o modem_test modem_test.cxx
