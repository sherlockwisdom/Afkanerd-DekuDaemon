#include "gl_modem_listener.cxx"
#include "gl_request_queue_listener.cxx"
using namespace std;

bool check_system_folders() {
	int result = mkdir(SYS_FOLDER.c_str(), STD_DIR_MODE);
	if(result == 0 || errno == EEXIST) cout << "check_system_folder=> $HOME/deku = DONE" << endl;
	else {
		char str_error[256];
		string error_message = strerror_r( errno, str_error, 256);
		cerr << "check_system_folder.error=> creating /deku/... " << error_message << endl;
		return false;
	}

	result = mkdir(SYS_FOLDER_MODEMS.c_str(), STD_DIR_MODE);
	if(result == 0 || errno == EEXIST) cout << "check_system_folder=> $HOME/deku/modems = DONE" << endl;
	else {
		char str_error[256];
		string error_message = strerror_r( errno, str_error, 256);
		cerr << "check_system_folder.error=> creating /modems/... " << error_message << endl;
		return false;
	}
	return true;
}

int main() {
	//TODO: thread a listener that creates a dir when a new modem is plugged in and updates system modem pool - done
	//TODO: thread a listener that listens for changes to request files and calculate work load and distributes to modems - done
	//TODO: thread listeners that listen to each modem folder and executes jobs for specific modem alone
	//main process spins of threads and manages them
	
	//checks and create defaults before begining the threads
	if( !check_system_folders()) {
		return 1;
	}


	std::thread tr_modem_listener(gl_modem_listener, "Master Modem Listener");
	std::thread tr_request_listener(gl_request_queue_listener, "Request Queue Listener");
	tr_modem_listener.join();
	tr_request_listener.join();
			
	return 1;
}
