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
		logger::logger_errno( errno );
		return false;
	}

	vector<string> sys_file_contents = helpers::read_file( path_to_sys_file );
	if( sys_file_contents.size() < 1 ) {
		logger::logger(__FUNCTION__, "Nothing to read in config file", "stderr", true);
		return false;
	}

	for(auto config_line: sys_file_contents) {
		logger::logger(__FUNCTION__, config_line);
		vector<string> configs = helpers::split(config_line, '=', true);
		if(configs[0] == "DIR_REQUEST_FILE") {
			string dir_request_file = configs[1];
			if(!helpers::file_exist( dir_request_file ) ) {
				helpers::make_dir( dir_request_file );
				logger::logger(__FUNCTION__, "CREATING DIR_REQUEST_FILE: "+dir_request_file);
			}
			else logger::logger(__FUNCTION__, "DIR_REQUEST_FILE already exist" );
		}
		else if(configs[0] == "DIR_ISP") {
			string dir_isp = configs[1];
			if(!helpers::file_exist( dir_isp ) ) {
				helpers::make_dir( dir_isp );
				logger::logger(__FUNCTION__, "CREATING DIR_ISP: "+dir_isp);
			}
			else logger::logger(__FUNCTION__, "DIR_ISP exist already exist" );
		}
		else if(configs[0] == "DIR_SCRIPTS") {
			string dir_scripts = configs[1];
			if(!helpers::file_exist( dir_scripts )) {
				logger::logger(__FUNCTION__, "PATH TO SCRIPT NOT VALID...", "stderr", true);
				return false;
			}
		}
	}

	return true;
}

map<string,string> get_system_configs( vector<string> sys_config_lines ) {
	map<string,string> configs;
	for(auto config_line: sys_config_lines) {
		vector<string> tmp_configs = helpers::split(config_line, '=', true);
		if(tmp_configs.size() > 1) configs.insert(make_pair( tmp_configs[0], tmp_configs[1]));
		else {
			logger::logger(__FUNCTION__, "Error reading configs...", "stderr", true);
		}
	}
	return configs;
}
#endif
