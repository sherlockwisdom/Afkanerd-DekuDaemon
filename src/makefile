CC = g++
CFLAGS = -g -Wall
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
