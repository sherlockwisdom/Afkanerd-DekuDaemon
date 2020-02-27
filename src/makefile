CC = g++
CFLAGS = -g -Wall -pthread -std=c++1z
TARGET = start

default: $(TARGET)

$(TARGET): $(TARGET).cxx
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cxx

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
