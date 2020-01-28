// Purpose: Starts all the functions that monitor the hardware and monitors the request files
#include "start_routines.hpp"
#include "formatters/helpers.hpp"
#include "sub-routines/request_distribution_listener.cxx"
//#include "sub-routines/request_execution_listener.cxx"
using namespace std;

int main(int argc, char** argv) {
	std::string PATH_SYS_FILE = "build_files/sys_file.txt";

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
	
	return 0;
}
