// Purpose: Starts all the functions that monitor the hardware and monitors the request files

#include <iostream>
#include <map>
#include "formatters/helpers.hpp"
using namespace std;


bool system_check( string path_to_sys_file) {
	logger::logger( __FUNCTION__, "Running system check at: " + path_to_sys_file, "stdout", true);

	if( !helpers::file_exist(path_to_sys_file)) {
		logger::logger(__FUNCTION__, "System file does not exist", "stderr", true);
		return false;
	}

	vector<string> sys_file_contents = helpers::read_file( path_to_sys_file );
	if( sys_file_contents.size() < 1 ) {
		//TODO: put logger statement
	}

	for(auto config_line: sys_file_contents) {
		vector<string> configs = helpers::split(config_line, ':');
		if(configs[0] == "DIR_REQUEST_FILE") {
			//TODO: check if dir exist, else create it
		}
		else if(configs[0] == "DIR_ISP") {
			//TODO: check if dir exist, else create it
		}
	}
}

map<string,string> get_system_configs( vector<string> sys_config_lines ) {
	map<string,string> configs;
	for(auto config_line: sys_config_lines) {
		//TODO: put integrity check to make sure valid configs have been entered
		vector<string> tmp_configs = helpers::split(config_line, ':');
		configs.insert(make_pair( tmp_configs[0], tmp_configs[1]));
	}
	return configs;
}


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
	
	return 0;
}
