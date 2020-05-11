#include <map>
#include <iostream>
#include <vector>
#include "formatters/helpers.hpp"
#include "sys_calls/sys_calls.hpp"
#include "../mysql/mysql.hpp"
#ifndef START_ROUTINES_H_INCLUDED_
#define START_ROUTINES_H_INCLUDED_
using namespace std;


bool system_check( string path_to_sys_file) {
	//TODO: Check if mysql database exist - if not create it
	//TODO: Check integrety of mysql database, if not create it

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

	string MYSQL_SERVER,
	       MYSQL_USER,
	       MYSQL_PASSWORD,
	       MYSQL_DATABASE;

	for(auto config_line: sys_file_contents) {
		logger::logger(__FUNCTION__, config_line);
		vector<string> configs = helpers::string_split(config_line, '=');
		for(auto config : configs ) logger::logger(__FUNCTION__, config);
		if(configs[0] == "DIR_REQUEST_FILE") {
			string dir_request_file = configs[1];
			if(!helpers::file_exist( dir_request_file ) ) {
				logger::logger(__FUNCTION__, "CREATING DIR_REQUEST_FILE: "+dir_request_file);
				helpers::make_dir( dir_request_file );
			}
			else logger::logger(__FUNCTION__, "DIR_REQUEST_FILE already exist" );
		}
		else if(configs[0] == "DIR_ISP") {
			string dir_isp = configs[1];
			logger::logger(__FUNCTION__, "Dir ISP: " + dir_isp);
			if(!helpers::file_exist( dir_isp ) ) {
				logger::logger(__FUNCTION__, "CREATING DIR_ISP: "+dir_isp);
				helpers::make_dir( dir_isp );
			}
			else logger::logger(__FUNCTION__, "DIR_ISP exist already exist" );
		}
		else if(configs[0] == "DIR_SCRIPTS") {
			string dir_scripts = configs[1];
			logger::logger(__FUNCTION__, "=> " + dir_scripts );
			if(!helpers::file_exist( dir_scripts )) {
				logger::logger(__FUNCTION__, "PATH TO SCRIPT NOT VALID...", "stderr", true);
				return false;
			}
		}
		else if(configs[0] == "DIR_SUCCESS") {
			string dir_success = configs[1];
			if(!helpers::file_exist( dir_success )) {
				logger::logger(__FUNCTION__, "CREATING DIR SUCCESS...", "stderr", true);
				sys_calls::make_dir( dir_success );
			}
			else logger::logger(__FUNCTION__, "DIR_SUCCESS already exist");
		}

		else if(configs[0] == "MYSQL_SERVER") {
			MYSQL_SERVER = configs[1];
		}
		else if(configs[0] == "MYSQL_USER") {
			MYSQL_USER = configs[1];
		}
		else if(configs[0] == "MYSQL_PASSWORD"){
			MYSQL_PASSWORD = configs[1];
		}
		else if(configs[0] == "MYSQL_DATABASE") {
			MYSQL_DATABASE = configs[1];
		}
	}

	if( MYSQL_SERVER.empty()) {
		logger::logger(__FUNCTION__, "MYSQL SERVER NOT SET", "stderr", true);
		return false;
	}
	
	if( MYSQL_USER.empty()) {
		logger::logger(__FUNCTION__, "MYSQL USER NOT SET", "stderr", true);
		return false;
	}

	if( MYSQL_PASSWORD.empty()) {
		logger::logger(__FUNCTION__, "MYSQL PASSWORD NOT SET", "stderr", true);
		return false;
	}

	if( MYSQL_DATABASE.empty()) {
		logger::logger(__FUNCTION__, "MYSQL DATABASE NOT SET", "stderr", true);
		return false;
	}

	//check if database exist
	MySQL mysql(MYSQL_SERVER, MYSQL_USER, MYSQL_PASSWORD);
	if( MYSQL_DATABASE.empty()) {
		logger::logger(__FUNCTION__, "MYSQL DATABASE NOT PROVIDED", "stderr", true);
		return false;
	}

	if( !mysql.has_database( MYSQL_DATABASE ) ) {
		if( mysql.create_database( MYSQL_DATABASE ) ) {
			logger::logger(__FUNCTION__, "MYSQL DATABASE CREATED", "stdout", true);
			mysql.set_database( MYSQL_DATABASE );
		}
		else {
			logger::logger(__FUNCTION__, "FAILED CREATING MYSQL DATABASE", "stderr", true);
			logger::logger(__FUNCTION__, mysql.get_error_message(), "stderr", true);
			logger::logger_errno( errno );
			return false;
		}
	}

	string DEKU_TABLE_MODEM_MONITOR = "MODEMS",
	       DEKU_TABLE_MODEM_SMS_STATUS = "MODEM_WORK_LOAD";

	string DEKU_MODEM_MONITOR_VALUES = "IMEI bigint(20) NOT NULL PRIMARY KEY,"
		"PHONENUMBER int(20) NULL,"
		"TYPE enum('ssh','mmcli') NOT NULL,"
		"STATE enum('active','exhausted') NOT NULL,"
		"POWER enum('plugged','not_plugged') NOT NULL,"
		"DATE timestamp DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP",

	       DEKU_MODEM_SMS_STATUS_VALUES = "IMEI bigint(20) NOT NULL,"
		       "WORK_LOAD INT(11) NOT NULL,"
		       "DATE DATE,"
		       "MDATE timestamp DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP";	

	if( !mysql.has_table( DEKU_DEFAULT_TALE ) ) {
		if( mysql.create_table( DEKU_TABLE_MODEM_MONITOR, DEKU_MODEM_MONITOR_VALUES ))
			logger::logger(__FUNCTION__, "MODEM MONITOR DATABASE CREATED", "stdout", true);
		else {
			logger::logger(__FUNCTION__, "FAILED CREATING MODEM MONITOR DATABASE TABLE", "stderr", true);
			logger::logger(__FUNCTION__, mysql.get_error_message(), "stderr", true);
			logger::logger_errno( errno );
			return false;
		}

		if( mysql.create_table( DEKU_TABLE_MODEM_SMS_STATUS, DEKU_MODEM_SMS_STATUS_VALUES ))
			logger::logger(__FUNCTION__, "MODEM SMS STATUS DATABASE CREATED", "stdout", true);
		else {
			logger::logger(__FUNCTION__, "FAILED CREATING SMS STATUS DATABASE TABLE", "stderr", true);
			logger::logger(__FUNCTION__, mysql.get_error_message(), "stderr", true);
			logger::logger_errno( errno );
			return false;
		}
	}

	return true;
}

map<string,string> get_system_configs( vector<string> sys_config_lines ) {
	map<string,string> configs;
	for(auto config_line: sys_config_lines) {
		vector<string> tmp_configs = helpers::string_split(config_line, '=');
		if(tmp_configs.size() > 1) {
			string value = tmp_configs[1];
			if(value[value.size() -1] == '/')
				value.erase(value.size() -1, 1);
			configs.insert(make_pair( tmp_configs[0], value));
		}
		else {
			logger::logger(__FUNCTION__, "Error reading configs...", "stderr", true);
		}
	}
	return configs;
}
#endif
