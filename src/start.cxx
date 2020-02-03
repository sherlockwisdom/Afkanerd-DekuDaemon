// Purpose: Starts all the functions that monitor the hardware and monitors the request files
#include "start_routines.hpp"
#include "formatters/helpers.hpp"
//#include "sub-routines/request_distribution_listener.cxx"
//#include "sub-routines/request_execution_listener.cxx"
#include "sub-routines/modem_listener.cxx"
using namespace std;


void user_input( Modems& modems ) {
	while( 1 ) {
		cout << __FUNCTION__ << ": ";
		string input;
		getline(cin, input);

		cout << __FUNCTION__<< " = " << input << endl;
	}
}

int main(int argc, char** argv) {
	//std::string PATH_SYS_FILE = "build_files/sys_file.txt";
	
	string PATH_SYS_FILE;
	if(argc < 2 ) {
		logger::logger(__FUNCTION__, "Usage: -c <path_to_config_file>", "stderr", true);
		return 1;
	}
	else {
		for(int i=1;i<argc;++i) {
			if((string)argv[i]== "-c") {
				logger::logger(__FUNCTION__, "config file arguments present", "stdout", false);
				if(i+1 < argc) {
					PATH_SYS_FILE = argv[i+1];
					++i;
				}
				else {
					logger::logger(__FUNCTION__, "Incomplete args\nUsage: -c <path_to_config_file>", "stderr", true);
					return 1;
				}
			}
		}
	}

	if(PATH_SYS_FILE.empty()) {
		logger::logger(__FUNCTION__, "Usage: -c <path_to_config_file>", "stderr", true);
		return 1;
	}

	// Begins by running the simple beginning test
	// TEST:
	// 1. Checks if system files are available
	
	if( !system_check( PATH_SYS_FILE )) {
		logger::logger( __FUNCTION__, "System check failed....", "stderr", true);
		return 1;
	}

	// Then after the checks, it moves set the variables for global use
	map<string,string> configs = get_system_configs( helpers::read_file( PATH_SYS_FILE ));

	// Begin listening for input (request file)
	//thread tr_request_distribution_listener( request_distribution_listener, configs);
	// Begin listening for request (modems)
	//thread tr_request_execution_listener( request_execution_listener, configs);

	//tr_request_distribution_listener.join();
	//tr_request_execution_listener.join();
	
	//Modems modems( Modems::PRODUCTION );
	Modems modems( Modems::TEST );

	std::thread listen_modems = std::thread(&Modems::__INIT__, std::ref(modems), configs);
	std::thread start_modems = std::thread(&Modems::startAllModems, std::ref(modems));
	std::thread tr_user_input = std::thread(user_input, std::ref(modems));
	listen_modems.join();
	start_modems.join();
	tr_user_input.join();
	
	return 0;
}
