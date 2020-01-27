#include <sys/inotify.h>
#include <iostream>
#include "formatters/logger.hpp"

using namespace std;

int main() {

	int i_init = inotify_init1(IN_NONBLOCK);
	if(i_init == -1 ) {
		cerr << "Failed to initialize inotify" << endl;
		return 1;
	}

	int i_watch = inotify_add_watch(i_init, "inotify.cxx", IN_MODIFY);
	if(i_watch == -1) {
		cerr << "Failed to watch file" << endl;
		logger::logger_errno( errno );
		return 1;
	}

	return 0;
}
