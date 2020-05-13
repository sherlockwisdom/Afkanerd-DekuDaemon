// Purpose: Starts all the functions that monitor the hardware and monitors the request files
#include "start_routines.hpp"
#include "sub-routines/modem_listener.hpp"
#include "sub-routines/request_distribution_listener.hpp"
using namespace std;

map<string, Modem*> Modems::available_modems = {};

void release_pending_request_files( map<string,string> configs, bool list_only = false) {
	string dir_isp = configs["DIR_ISP"];
	
	vector<string> isp_dirs = helpers::string_split(sys_calls::terminal_stdout("ls -1 " + dir_isp), '\n');

	for(auto dir : isp_dirs ) {
		if( dir.empty() ) continue;
		string full_dir = dir_isp + "/" + dir + "/";
		// sys_calls::terminal_stdout("rm -r " + full_dir + "*");
		
		// Get hidden files
		string locked_files_request = configs["DIR_SCRIPTS"] + "/modem_information_extraction.sh list_locked_request_files " + full_dir;
		string list_terminal_files = sys_calls::terminal_stdout( locked_files_request );
		// logger::logger(__FUNCTION__, list_terminal_files);
		vector<string> locked_files = helpers::string_split( list_terminal_files, '\n');

		if( locked_files.empty()) {
			logger::logger(__FUNCTION__, "NO LOCKED FILES!!", "stdout", true);
			continue;
		}
		if( !list_only ) {
			logger::logger(__FUNCTION__, "RELEASING: " + full_dir );
			for( auto file : locked_files ) {
				string locked_file = full_dir + file;
				file.erase(0, 1);
				string unlocked_file = full_dir + file;
				if( !sys_calls::rename_file( locked_file, unlocked_file ) ) {
					logger::logger(__FUNCTION__, "FAILED UNLOCKING FILE: " + locked_file + " -> " + unlocked_file, "stderr", true);
					logger::logger_errno( errno );
					continue;
				}

				logger::logger(__FUNCTION__, "UNLOCKED FILE: " + unlocked_file, "stdout", true);
			}
		}
		else {
			logger::logger(__FUNCTION__, "LISTING: " + full_dir);
			for( auto file: locked_files ) {
				string locked_file = full_dir + file;
				logger::logger(__FUNCTION__, locked_file, "stdout", true);
			}
		}
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

void request_cleanse( map<string,string> configs ) {
	string dir_isp = configs["DIR_ISP"];
	
	vector<string> isp_dirs = helpers::string_split(sys_calls::terminal_stdout("ls -1 " + dir_isp), '\n');

	for(auto dir : isp_dirs ) {
		if( dir.empty() ) continue;
		string full_dir = dir_isp + "/" + dir + "/";
		logger::logger(__FUNCTION__, "ClEANSING: " + full_dir );
		sys_calls::terminal_stdout("rm -r " + full_dir + "*");
	}
}

map<string,string> parse_ussd_request_script( string request_script ) {
	//sample looks like 
	//isp=MTN,retry_count=3,command=*155#
	
	map<string,string> parsed_commands;
	
	vector<string> tokens = helpers::string_split( request_script, ',');
	for( auto token : tokens ) {
		vector<string> split_token = helpers::string_split( token, '=' );
		if( split_token.size() < 1 ) return map<string,string>{};

		if( split_token[0] == "isp") {
			parsed_commands.insert(make_pair("isp", split_token[1]));
		}
		else if( split_token[0] == "retry_count") {
			parsed_commands.insert(make_pair("retry_count", split_token[1]));
		}
		else if( split_token[0] == "command") {
			parsed_commands.insert(make_pair("command", split_token[1]));
		}
	}

	return parsed_commands;
}

int main(int argc, char** argv) {
	// Default values
	Modems::STATE RUNNING_MODE = Modems::TEST;
	string PATH_SYS_FILE;
	string custom_filename;

	int quantity_to_generate = 0;
	int sleep_time = 10; // 10 seconds
	// int exhaust_count = 3; // 10 seconds

	bool cleanse = false, 
	     cleanse_only = false, 
	     stat_only = false,
	     sms_only = false,
	     request_listening = true,
	     list_locked_files_only = false,
	     release_locked_files_only = false;

	// SMS system set to create

	map<string,string> ussd_only_script;

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
					if( helpers::to_uppercase(mode) == "PRODUCTION") {
						RUNNING_MODE = Modems::PRODUCTION;
						logger::show_state = "PRODUCTION";
					}
					else {
						RUNNING_MODE = Modems::TEST;
						logger::show_state = "TEST";
					}
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

			/*
			else if((string)argv[i] == "--exhaust_count") {
				if(i+1< argc) {
					//exhaust_count = atoi(((string)argv[i+1]).c_str());
					if( sleep_time < 1 ) {
						logger::logger(__FUNCTION__, "min exhaust count = 1 tries, setting default to 3", "stderr", true);
					//	exhaust_count = 3; //todo: change - default should come from config file
					}
				}
				logger::logger(__FUNCTION__, "Setting Modem Exhausted at: " + (string)argv[i+1] + " tries", "stdout", true);
			}
			*/

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

			else if((string)argv[i] == "--cleanse-only") {
				cleanse_only = true;
			}

			else if((string)argv[i] == "--reboot") {
				sys_calls::sys_reboot();
			}

			else if((string)argv[i] == "--stat-only") {
				stat_only = true;
			}

			else if((string)argv[i] == "--ussd-only") {
				if(i+1 < argc) {
					++i;

					string check_script = ((string)argv[i]).substr(0, ((string)("--script=")).size());
					if(check_script != "--script="){
						logger::logger(__FUNCTION__, "--script= Not found in args", "stderr", true);
						return 1;
					}
					//Parse script
					string script_request = ((string)argv[i]).substr(((string)("--script=")).size(), ((string)(argv[i])).size());
					ussd_only_script = parse_ussd_request_script( script_request );
					if( ussd_only_script.empty() ) {
						logger::logger(__FUNCTION__, "Script cannot be empty", "stderr", true);
						return 1;
					}
				}
				else {
					logger::logger(__FUNCTION__, "Incomplete args\nUsage: --ussd-only --script", "stderr", true);
					return 1;
				}

			}

			else if((string)argv[i] == "--sms-only") {
				sms_only = true;
				request_listening = false;
			}

			else if((string)argv[i] == "--sms") {
				sms_only = true; // Not changing this cus request_listening is the defining factor
			}

			else if((string)argv[i] == "--ls-locked-files-only") {
				list_locked_files_only = true;
			}

			else if((string)argv[i] == "--release-locked-files-only") {
				release_locked_files_only = true;
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
	Modems modems( configs, RUNNING_MODE );

	// Generate test request TODO: Make better so dynamic test messages can be passed into system
	if( quantity_to_generate > 0 ) {
		generate_request( configs, quantity_to_generate );
	}

	if( list_locked_files_only ) {
		logger::logger(__FUNCTION__, "LISTING LOCKED FILES", "stdout", true);
		release_pending_request_files( configs, true);
		return 0;
	}

	if( release_locked_files_only ) {
		logger::logger(__FUNCTION__, "RELEASING LOCKED FILES", "stdout", true);
		release_pending_request_files( configs );
		return 0;
	}

	if( release_locked_files_only ) {
		logger::logger(__FUNCTION__, "RELEASING LOCKED FILES", "stdout", true);
		release_pending_request_files( configs );
		return 0;
	}

	if( cleanse_only ) {
		logger::logger(__FUNCTION__, "COMMENSING A CLEANSE", "stdout", true);
		request_cleanse( configs );
		return 0;
	}

	if( cleanse ) {
		logger::logger(__FUNCTION__, "COMMENSING A CLEANSE", "stdout", true);
		request_cleanse( configs );
	}

	if( stat_only ) {
		logger::logger(__FUNCTION__, "=======> MODEM STATS", "stdout", true);
		auto available_modems = modems.get_available_modems();
		if( available_modems.empty()) {
			logger::logger(__FUNCTION__, "NO AVAILABLE MODEMS", "stdout", true);
			return 0;
		}
		for( auto modem_ : available_modems ) {
			auto modem_details = modems.get_modem_details( modem_.second );
			
			if( modem_details.empty() ) continue;;

			Modem modem( modem_details["imei"], modem_details["isp"], modem_details["type"], modem_details["index"], configs);

			logger::logger("> STAT: ", modem.getInfo(), "stdout", true);
		}
		return 0;
	}

	if( !ussd_only_script.empty()) {
		logger::logger(__FUNCTION__, "EXECUTING USSD SCRIPTS", "stdout", true);
		if( ussd_only_script.find("command") == ussd_only_script.end()) {
			logger::logger(__FUNCTION__, "No Command available", "stdout", true);
			return 1;
		}
		if( ussd_only_script.find("retry_count") == ussd_only_script.end()) {
			logger::logger(__FUNCTION__, "Setting retry count to 0", "stdout", true);
			ussd_only_script.insert(make_pair("retry_count", "0"));
		}
		vector<Modem*> available_modems = modems.find_modem_type(ussd_only_script["isp"]);
		logger::logger(__FUNCTION__, "Available Modems: " + to_string( available_modems.size()), "stdout", true);

		int retry_count = 0;
		for( size_t it_modems = 0; it_modems < available_modems.size(); ++it_modems) {
			vector<string> commands = helpers::string_split( ussd_only_script["command"], '|' );
			bool ussd_state = available_modems[it_modems]->initiate_series( commands );
			if( !ussd_state and retry_count <= atoi(((string)(ussd_only_script["retry_count"])).c_str()) ) {
				logger::logger(__FUNCTION__, available_modems[it_modems]->getInfo() + "| " + available_modems[it_modems]->get_reply(), "stderr", true);
				++retry_count;
				continue;
			}

			logger::logger(__FUNCTION__, available_modems[it_modems]->get_response());
			++it_modems;
			retry_count = 0;
		}

		return 0;
	}

	
	// TODO: Check if other developers variables are passed as args and set before beginning


	// TODO: Pass all configs using refreences, so changes get loaded in real time
	std::thread tr_modems_scanner = std::thread(&Modems::begin_scanning, std::ref(modems), request_listening, sms_only);
	
	std::thread tr_request_listeners = std::thread(request_distribution_listener::request_distribution_listener, configs);

	tr_modems_scanner.join();
	tr_request_listeners.join();
	
	return 0;
}
