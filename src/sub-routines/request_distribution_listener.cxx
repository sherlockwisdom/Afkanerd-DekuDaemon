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
	

	while( 1 ) {
		if( helpers::check_file( PATH_REQUEST_FILE ) ) {
			//TODO: rename file and parse it
			string random_name = config["DIR_REQUEST_FILE"] + "/" + helpers::random_string();
			if( !helpers::rename_file( PATH_REQUEST_FILE, random_name )) {
				logger::logger_errno(errno);
			}
			else {
				vector<string> requests = helpers::read_file(random_name);
				for(auto request : requests) {
					vector<string> request_extract = parsers::comma_seperation( request );
					for(auto r_entity : request_extract ) {
						//TODO: critical... if equals is in message, it will parse through the message
						vector<string> component = parsers::equal_seperate( r_entity );
						if( component[0] == "number" ) number = component[1];
						else if(component[0] == "message" ) message = component[1];
					}
					string isp = isp_determiner::get_isp( number );
					//TODO: check if ISP folder exist...
					if( !helpers::file_exist( config["DIR_ISP"] + "/" + isp + "/" ) ) {
						helpers::make_dir(config["DIR_ISP"] + "/" + isp);
					}
					//TODO: send new file to isp distributor
					//TODO: isp distributor parses file and share among isp folders
					//TODO: sends confirmation message and begins again

					//TODO: Moving file to the ISP folder
					helpers::rename( random_name, config["DIR_ISP"] + "/" + isp + "/" + helpers::random_string());
				}
			}
		}

	}
	
}


