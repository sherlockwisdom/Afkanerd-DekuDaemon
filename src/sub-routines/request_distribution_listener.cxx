//TODO: Keeps event listener for changes to request file
//TODO: Parses request file
//TODO: send parsed content to isp distributor
#include <map>
#include <algorithm>
#include "../formatters/helpers.hpp"
#include "../parsers/comma_equals_parser.hpp"
#include "../sub-routines/isp_determiner.hpp"
using namespace std;

namespace request_distribution_listener {
	bool configs_check( map<string,string> configs ) {
		//TODO: put the things one needs to check in here
		return (
				configs.find("DIR_ISP") != configs.end() and 
				configs.find("DIR_REQUEST_FILE") != configs.end() and 
				configs.find("STD_NAME_REQUEST_FILE") != configs.end()
		);
	}

	string isp_distributor( string message, string number, map<string,string> config ) {
		string isp = isp_determiner::get_isp( number );
		string request = "number="+number+",message=\""+message+"\"";
		logger::logger(__FUNCTION__, "Distributing [" + request + "] - [" + isp + "]");
		if( !helpers::file_exist( config["DIR_ISP"] + "/" + isp + "/" ) ) {
			helpers::make_dir(config["DIR_ISP"] + "/" + isp);
		}
		helpers::write_file( config["DIR_ISP"] + "/" + isp + "/" + helpers::random_string(), request, false, ios::trunc);
		return isp;
	}

	map<string,string> request_parser(string request) {
		map<string, string> extracted_request;
		vector<string> request_extract = parsers::comma_seperate( request );
		string message, number;
		for(auto r_entity : request_extract ) {
			//TODO: critical... if equals is in message, it will parse through the message
			vector<string> component = parsers::equal_seperate( r_entity );
			if( component[0] == "number" ) extracted_request.insert(make_pair("number", component[1]));
			else if(component[0] == "message" ) {
				if(component[1][0] == '"') component[1].erase(0,1);
				if(component[1][component[1].size()-1] == '"') component[1].erase(component[1].size()-1,1);
				message = component[1];
				extracted_request.insert(make_pair("message", message));
			}
		}

		return extracted_request;
	}

	void request_distribution_listener( map<string, string> configs ) {
		//TODO: run system checks here
		if( !configs_check(configs)) {
			logger::logger( __FUNCTION__, "Configuration check failed, cannot start", "stderr", true);
			return;
		}

		string PATH_REQUEST_FILE = configs["DIR_REQUEST_FILE"] + "/" + configs["STD_NAME_REQUEST_FILE"];
		while( 1 ) {
			if( helpers::file_exist( PATH_REQUEST_FILE ) ) {
				//TODO: rename file and parse it
				logger::logger(__FUNCTION__, "Request file is present..", "stdout", true);
				string random_name = configs["DIR_REQUEST_FILE"] + "/" + helpers::random_string();
				if( !sys_calls::rename_file( PATH_REQUEST_FILE, random_name )) {
					logger::logger(__FUNCTION__, "Not good, couldn't lock request file...", "stderr", true);
					logger::logger_errno(errno);
				}
				else {
					vector<string> requests = helpers::read_file(random_name);
					for(auto request : requests) {
						map<string,string> extracted_request = request_parser( request );
						string number = extracted_request["number"];
						string message = extracted_request["message"];
						isp_distributor( message, number, configs );
					}
				}
			}
			else {
				logger::logger(__FUNCTION__, "No current request...");
			}

			helpers::sleep_thread( 10 ); //Stops CPU from running on overclock
			//TODO: use inotify to notify changes in directory structure and files
		}
		
	}

}
