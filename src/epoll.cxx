#include <sys/epoll.h>
#include <iostream>
#include "formatters/helpers.hpp"

using namespace std;


int main() {
	int epoll_fd = epoll_create1(0);

	if(epoll_fd == -1) {
		cout << "Failed to create epoll descriptor" << endl;
	}

	cout << "epoll value: " << epoll_fd << endl;

	struct epoll_event event;

	event.events = EPOLLIN;
	event.data.fd = 0;

	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event) == -1) {
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
