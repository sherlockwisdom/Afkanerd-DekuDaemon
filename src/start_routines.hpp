#include <map>
#include <iostream>
#include <vector>
#include "formatters/helpers.hpp"
#ifndef START_ROUTINES_H_INCLUDED_
#define START_ROUTINES_H_INCLUDED_
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
			string dir_request_file = configs[1];
			if(!helpers::file_exist( dir_request_file ) ) {
				helpers::make_dir( dir_request_file );
				logger::logger(__FUNCTION__, "CREATING DIR_REQUEST_FILE");
			}
			else logger::logger(__FUNCTION__, "DIR_REQUEST_FILE exist" );
		}
		else if(configs[0] == "DIR_ISP") {
			//TODO: check if dir exist, else create it
			string dir_isp = configs[1];
			if(!helpers::file_exist( dir_isp ) ) {
				helpers::make_dir( dir_isp );
				logger::logger(__FUNCTION__, "CREATING DIR_ISP");
			}
			else logger::logger(__FUNCTION__, "DIR_ISP exist" );
		}
		else if(configs[0] == "STD_NAME_REQUEST_FILE") {
			//TODO: this seems fine for now
			string std_name_request_file = configs[1];
			if(!helpers::file_exist( std_name_request_file ) ) {
				helpers::make_dir( std_name_request_file );
				logger::logger(__FUNCTION__, "CREATING STD_NAME_REQUEST_FILE");
			}
			else logger::logger(__FUNCTION__, "DIR_ISP STD_NAME_REQUEST_FILE" );
		}
	}
}

map<string,string> get_system_configs( vector<string> sys_config_lines ) {
	//TODO: UTest this function
	map<string,string> configs;
	for(auto config_line: sys_config_lines) {
		//TODO: put integrity check to make sure valid configs have been entered
		vector<string> tmp_configs = helpers::split(config_line, ':', true);
		configs.insert(make_pair( tmp_configs[0], tmp_configs[1]));
	}
	return configs;
}
#endif
