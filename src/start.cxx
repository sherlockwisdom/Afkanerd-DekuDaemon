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


void generate_request( map<string,string> configs, int quantity_to_generate ) {
	// TODO: Put more work in here, cus fuck it... it's still got a private number lol
	logger::logger(__FUNCTION__, "Generating " + to_string( quantity_to_generate ) + " request", "stdout", true);
	string request = "";
	string path_to_request_file = configs["DIR_REQUEST_FILE"] + "/" + configs["STD_NAME_REQUEST_FILE"];
	logger::logger(__FUNCTION__, "Path to generated request file: " + path_to_request_file);
	for(int i=0;i<quantity_to_generate;++i) {
		// TODO: number should come from the args passed in the CLI
		request += "number=652156811,message=\"AfkanerdDevelopers\\n2020-02-03\\n52515\\nSHERLOCK2\\nFCs Test Region, Afkanerd Developers\\nAFB, 1+\\n12345\\nXpert, MTB DETECTED (LOW) RIF resistance indeterminate\\nURINE LF-LAM, NEGATIVE\\n\\nHelpline/Ligne 670656041\"\n";
		// TODO: This has the french symbols in it
		// request += "number=652156811,message=\"TAYONG GABRIEL\\n999999999\\nBAMENDA RH\\nMTB DETECTED\\nMon Mar 09\\n2020\\nNot yet recorded as starting TB Rx\\nN’est pas enregistrer a commencer le T3 TB\\nHelpline 656659119\"\n";

		// TODO: French symbols removed from this one
		// request += "number=652156811,message=\"TAYONG GABRIEL\\n999999999\\nBAMENDA RH\\nMTB DETECTED\\nMon Mar 09\\n2020\\nNot yet recorded as starting TB Rx\\nN est pas enregistrer a commencer le T3 TB\\nHelpline\"\n";
	}
	helpers::write_file( path_to_request_file, request );
}

void cleanse( map<string,string> configs ) {
	string dir_isp = configs["DIR_ISP"];
	
	vector<string> isp_dirs = helpers::string_split(sys_calls::terminal_stdout("ls -1 " + dir_isp), '\n');

	for(auto dir : isp_dirs ) {
		string full_dir = dis_isp + "/" + dir + "/";
		logger::logger(__FUNCTION__, "ClEANSING: " + full_dir );
		sys_calls::terminal_stdout("rm -r " + full_dir + "*");
	}
}

int main(int argc, char** argv) {
	// Default values
	Modems::STATE RUNNING_MODE = Modems::TEST;
	string PATH_SYS_FILE;
	string custom_filename;

	int quantity_to_generate = 0;
	int sleep_time = 10; // 10 seconds
	int exhaust_count = 3; // 10 seconds



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

			else if((string)argv[i] == "--show_isp") {
				if(i+1 < argc ) {
					string number = (string)argv[i+1];
					cout << isp_determiner::get_isp( number ) << endl;
					return 0;
				}
				else {
					return 1;
				}
			}

			else if((string)argv[i] == "--mode") {
				if(i+1 < argc ) {
					string mode = (string)argv[i+1];
					if( helpers::to_uppercase(mode) == "PRODUCTION") RUNNING_MODE = Modems::PRODUCTION;
					else RUNNING_MODE = Modems::TEST;
				}
			}

			else if((string)argv[i] == "--st") {
				if(i+1< argc) {
					sleep_time = atoi(((string)argv[i+1]).c_str());
					if( sleep_time < 3 ) {
						logger::logger(__FUNCTION__, "min sleep time = 3 seconds, setting default to 10", "stderr", true);
						sleep_time = 10; //todo: change - default should come from config file
					}
					logger::logger(__FUNCTION__, "Setting Modem sleep time at: " + (string)argv[i+1] + " seconds", "stdout", true);
				}
			}

			else if((string)argv[i] == "--exhaust_count") {
				if(i+1< argc) {
					exhaust_count = atoi(((string)argv[i+1]).c_str());
					if( sleep_time < 1 ) {
						logger::logger(__FUNCTION__, "min exhaust count = 1 tries, setting default to 3", "stderr", true);
						exhaust_count = 3; //todo: change - default should come from config file
					}
				}
				logger::logger(__FUNCTION__, "Setting Modem Exhausted at: " + (string)argv[i+1] + " tries", "stdout", true);
			}

			else if((string)argv[i] == "--generate_request") {
				if(i+1< argc) {
					quantity_to_generate = atoi(((string)argv[i+1]).c_str());
				}
				logger::logger(__FUNCTION__, "Number of request to generate: " + (string)argv[i+1] + " tries", "stdout", true);
			}

			// TODO: finish working on this
			else if((string)argv[i]== "-f") {
				if(i+1 < argc) {
					custom_filename = argv[i+1];
					++i;
				}
				else {
					logger::logger(__FUNCTION__, "Incomplete args\nUsage: -f <path_to_config_file>", "stderr", true);
					return 1;
				}
			}

			else if((string)argv[i] == "--cleanse") {
				cleanse = true;
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
	for(auto i : configs ) logger::logger("STARTING ROUTINES - [CONFIGS]:", i.first + "=" + i.second, "stdout", true);

	// Generate test request TODO: Make better so dynamic test messages can be passed into system
	if( quantity_to_generate > 0 ) {
		generate_request( configs, quantity_to_generate );
	}

	if( cleanse ) {
		logger::logger(__FUNCTION__, "COMMENSING A CLEANSE", "stdout", true);
		cleanse( configs );
	}

	//Modems modems( Modems::PRODUCTION );
	Modems modems( configs, RUNNING_MODE );
	
	// TODO: Check if other developers variables are passed as args and set before beginning


	// TODO: Pass all configs using refreences, so changes get loaded in real time
	std::thread tr_modems_scanner = std::thread(&Modems::begin_scanning, std::ref(modems));
	
	// std::thread tr_modem_starter = std::thread(&Modems::startAllModems, std::ref(modems));
	// std::thread tr_user_input = std::thread(user_input, std::ref(modems));
	std::thread tr_request_listeners = std::thread(request_distribution_listener::request_distribution_listener, configs);
	// tr_modem_listeners.join();
	// tr_modem_starter.join();
	// tr_user_input.join();
	tr_modems_scanner.join();
	tr_request_listeners.join();
	
	return 0;
}
