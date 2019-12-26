#ifndef DECLARATIONS_H_INCLUDED_
#define DECLARATIONS_H_INCLUDED_

#include <iostream>
#include <cstdio> //Testing if threads work in same dir
#include <vector>
#include <chrono>
#include <map>
#include <fstream>
#include <string>
#include <set>

#include "helpers.hpp"

using namespace std;

map<string,string> MODEM_INFORMATION_EXTRACTION_SCRIPT = {
	{"DEVELOPMENT", "./modem_information_extraction.sh"}, 
	{"TESTING", "./modem_information_mimic.sh"},
	{"PRODUCTION", "./modem_information_extraction.sh"}
};

string SYSTEM_STATE = "DEVELOPMENT";

string GET_MODEM_INFOR = [] {
	return MODEM_INFORMATION_EXTRACTION_SCRIPT[ SYSTEM_STATE ];
}

/* GLOBAL SYSTEM DECLARATIONS */
bool GL_MODEM_LISTENER_STATE = true;
bool GL_SYSTEM_READY = false;
int GL_TR_SLEEP_TIME = 5;
int GL_MMCLI_MODEM_SLEEP_TIME = 30; //Seconds

string GL_SSH_IP_GATEWAY = "192.168.";
string GL_TCP_HOST = "localhost";
string GL_TCP_PORT = "3000";
string GL_TCP_URL = "deku_daemon/message";
string ENV_HOME = getenv("HOME");
string SYS_FOLDER = ENV_HOME + "/deku";
string SYS_FOLDER_MODEMS = SYS_FOLDER + "/modems";
string SYS_ISP_DISTRIBUTION = SYS_FOLDER + "/isp_load_distribution";
string SYS_REQUEST_FILE = SYS_FOLDER + "/request_queue.dat";
string SYS_JOB_FILE = SYS_FOLDER + "/current_job.dat";

mode_t STD_DIR_MODE = 0777;

//INDEX -> <IMEI,LOADCOUNT>
map<string, vector<string>> MODEM_POOL;


//IMEI -> load count
map<string, int> MODEM_WORKLOAD;


//IMEI -> ISP
map<string, string> MODEM_DAEMON;

//IMEI -> SUCCESS COUNT
map<string, int> GL_SUCCESS_MODEM_LIST;

#endif
