CC = g++
CFLAGS = -g -Wall -pthread -std=c++1z
TARGET = start
MYSQL_CONFIGS := $(shell mysql_config --cflags)
MYSQL_LIBS := $(shell mysql_config --libs)
REQUIRED_FILES := sub-routines/modem_listener.cxx sub-routines/modem.cxx sub-routines/request_distribution_listener.cxx 

default: $(TARGET)

$(TARGET): $(TARGET).cxx
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cxx $(REQUIRED_FILES) $(MYSQL_CONFIGS) $(MYSQL_LIBS)

epoll: epoll.cxx
	$(CC) $(CFLAGS) -o epoll epoll.cxx

inotify: inotify.cxx
	$(CC) $(CFLAGS) -o inotify inotify.cxx

utest: UTest.cxx
	$(CC) $(CFLAGS) -o utest UTest.cxx
clean:
	rm -rf test_dir*/* epoll inotify utest $(TARGET)
start_test: start_test.cxx
	$(CC) $(CFLAGS) -o start_test start_test.cxx
mem_leak: mem_leak.cxx
	$(CC) $(CFLAGS) -o mem_leak mem_leak.cxx
mem_leak1: mem_leak1.cxx
	$(CC) $(CFLAGS) -o mem_leak1 mem_leak1.cxx
