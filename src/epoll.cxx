#include <sys/epoll.h>
#include <fcntl.h> //Allows open()
#include <iostream>
#include "formatters/helpers.hpp"

using namespace std;

int main() {
	int epoll_fd = epoll_create(256);

	if(epoll_fd == -1) {
		cout << "Failed to create epoll descriptor" << endl;
	}

	cout << "epoll value: " << epoll_fd << endl;

	struct epoll_event event;

	event.events = EPOLLIN;
	int custom_fd = open("epoll.cxx", O_RDONLY);
	if(custom_fd < 0) {
		perror("open");
	}
	//event.data.fd = custom_fd;

	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, custom_fd, &event) == -1) {
		cout << "Failed to add new interest list to epoll's libs..." << endl;
		logger::logger_errno( errno );
	}

	while(1) {
		int event_count = epoll_wait( epoll_fd, &event, 10, -1);
		if(event_count == -1) {
			cout << "Failed to wait for events" << endl;
			logger::logger_errno( errno );
		}
		cout << "event_count: " << event_count << endl;
	}

	cout << "I'm waiting for something.." << endl;

	return 0;
}
