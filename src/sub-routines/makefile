CC = g++
CFLAGS = -g -Wall -std=c++1z
TARGET = isp_determiner_test

default: $(TARGET)

$(TARGET): $(TARGET).cxx
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cxx

ussd_test: ussd_test.cxx
	$(CC) $(CFLAGS) -o ussd_test ussd_test.cxx
