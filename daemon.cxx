//#include "gl_modem_listener.cxx"
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

	result = mkdir(SYS_ISP_DISTRIBUTION.c_str(), STD_DIR_MODE);
	if(result == 0 || errno == EEXIST) cout << "check_system_folder=> " << SYS_ISP_DISTRIBUTION<< " = DONE" << endl;
	else {
		char str_error[256];
		string error_message = strerror_r( errno, str_error, 256);
		cerr << "check_system_folder.error=> creating /modems/... " << error_message << endl;
		return false;
	}
	return true;
}

int main( int argc, char** argv ) {

	// Determines if script should run
	bool start_daemon = false;

	//check to make sure
	if( argc < 2 ) {
		fprintf( stderr, "=> this isn't how to start this script...\n" );
		return 1;
	}

	for( int i = 1; i < argc ; ++i) {
		string arg = argv[i];
		if( arg == "--stats" ) { 
			//TODO: show stats and kill the script
			cout << "=> statistics------" << endl;
		}

		else if( arg == "--hw-info" ) {
			//TODO: show information about hardware plugged in
			cout << "=> hardware information" << endl;
		}
		
		else if( arg == "--start" ) start_daemon = true;

		else if( arg == "--state" ) {
			logger::show_state = argv[i+1];
			++i;
		}
	}

	logger::logger("STARTUP INFO", "\nlogger-state: " + logger::show_state + "\n\n", "stdout", true);


	if( !start_daemon ) return 0;

	if( !check_system_folders() ) {
		return 1;
	}

	std::thread start_services( daemon_function_for_threading );
	start_services.join();

	//std::thread tr_modem_listener(gl_modem_listener, "Master Modem Listener");

	//std::thread tr_request_listener(gl_request_queue_listener, "Request Queue Listener");

	//tr_modem_listener.join();
	//tr_request_listener.join();
			
	return 0;
}
