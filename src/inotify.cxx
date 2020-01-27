#include <sys/inotify.h>
#include <iostream>

using namespace std;

int main() {

	int i_init = inotify_init1(IN_NONBLOCK);
	if(i_init == -1 ) {
		cerr << "Failed to initialize inotify" << endl;
		return 1;
	}


}
