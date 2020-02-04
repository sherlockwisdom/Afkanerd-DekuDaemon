// Purpose: Starts all the functions that monitor the hardware and monitors the request files
#include "start_routines.hpp"
#include "formatters/helpers.hpp"
//#include "sub-routines/request_distribution_listener.cxx"
//#include "sub-routines/request_execution_listener.cxx"
#include "sub-routines/modem_listener.cxx"
using namespace std;


void user_input( Modems& modems ) {
	/* TODO: commands to introduce...

	isp stats = provide information about all isp
	"isp" stats = provide information about "isp"

	isp stats pending/locked = provide information about pending/locked jobs for all isp
	"isp" stats pending/locked = provide information about pending/locked jobs for "isp"
	*/

	while( 1 ) {
		cout << __FUNCTION__ << ": ";
		string input;
		getline(cin, input);

		cout << __FUNCTION__<< " = " << input << endl;
	}
}

int main(int argc, char** argv) {
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
	if( !system_check( PATH_SYS_FILE )) {
		logger::logger( __FUNCTION__, "System check failed....", "stderr", true);
		return 1;
	}

	// Then after the checks, it moves set the variables for global use
	map<string,string> configs = get_system_configs( helpers::read_file( PATH_SYS_FILE ));

	//Modems modems( Modems::PRODUCTION );
	Modems modems( Modems::TEST );

	//TODO: Pass all configs using refreences, so changes get loaded in real time
	std::thread tr_modem_listeners = std::thread(&Modems::__INIT__, std::ref(modems), configs);
	std::thread tr_modem_starter = std::thread(&Modems::startAllModems, std::ref(modems));
	std::thread tr_user_input = std::thread(user_input, std::ref(modems));
	std::thread tr_request_listeners = std::thread(request_distribution_listener::request_distribution_listener, configs);
	tr_modem_listeners.join();
	tr_modem_starter.join();
	tr_user_input.join();
	tr_request_listeners.join();
	
	return 0;
}
