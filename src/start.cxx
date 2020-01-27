// Purpose: Starts all the functions that monitor the hardware and monitors the request files

#include <iostream>
#include "formatters/logger.hpp"
using namespace std;

std::string PATH_SYS_FILE = "build_files/sys_file.txt";

bool system_check( string path_to_sys_file) {
	logger::logger( __FUNCTION__, "Running system check at: " + path_to_sys_file, "stdout", true);
}


int main(int argc, char** argv) {
	// Begins by running the simple beginning test
	// TEST:
	// 1. Checks if system files are available
	
	if( !system_check( PATH_SYS_FILE )) {
		logger::logger( __FUNCTION__, "System check failed....", "stderr", true);
	}
	
	return 0;
}
