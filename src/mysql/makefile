CC = g++
CFLAGS = -g -Wall -pthread -std=c++1z -c 
MYSQL_CONFIGS := $(shell mysql_config --cflags)
MYSQL_LIBS := $(shell mysql_config --libs)
TARGET = main
MYSQL_OBJECTS = mysql

default: $(TARGET)
mysql_object: $(MYSQL_OBJECTS)

$(TARGET): $(TARGET).cxx
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cxx $(MYSQL_CONFIGS) $(MYSQL_LIBS)

$(MYSQL_OBJECTS): $(MYSQL_OBJECTS).cxx
	$(CC) $(CFLAGS) -o $(MYSQL_OBJECTS) $(MYSQL_OBJECTS).cxx $(MYSQL_CONFIGS) $(MYSQL_LIBS)
