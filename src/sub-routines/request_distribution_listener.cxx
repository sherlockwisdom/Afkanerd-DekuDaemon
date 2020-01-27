//TODO: Keeps event listener for changes to request file
//TODO: Parses request file
//TODO: send parsed content to isp distributor


#include "request_distribution_listener.hpp"

using namespace std;


bool configs_check( map<string,string> configs ) {
	//TODO: put the things one needs to check in here
}

void request_distribution_listener( map<string, string> configs ) {
	//TODO: run system checks here
	if( !configs_check(configs)) {
		logger::logger( __FUNCTION__, "Configuration check failed, cannot start", "stderr", true);
		return;
	}

	PATH_REQUEST_FILE = configs["DIR_REQUEST_FILE"] + "/" + configs["STD_NAME_REQUEST_FILE"];
	//TODO: Add an event listener - libevents, epoll or just do polling ( yish )
	//TODO: if isp for request does not have a dir, to keep the required input, create this
	
	
}


