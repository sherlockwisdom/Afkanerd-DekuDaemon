#include "request_distribution_listener.hpp"
using namespace std;

namespace request_distribution_listener {
	bool configs_check( map<string,string> configs ) {
		return (
				configs.find("DIR_ISP") != configs.end() and 
				configs.find("DIR_REQUEST_FILE") != configs.end() and 
				configs.find("STD_NAME_REQUEST_FILE") != configs.end() and
				configs.find("DIR_SCRIPTS") != configs.end()
		);
	}

	string isp_distributor( string message, string number, string id, map<string,string> config ) {
		string isp = isp_determiner::get_isp( number );
		string request = "id="+id+",number="+number+",message=\""+message+"\"";
		logger::logger(__FUNCTION__, "Distributing [" + request + "] - [" + isp + "]");
		if( !helpers::file_exist( config["DIR_ISP"] + "/" + isp + "/" ) ) {
			helpers::make_dir(config["DIR_ISP"] + "/" + isp);
		}
		helpers::write_file( config["DIR_ISP"] + "/" + isp + "/" + helpers::random_string(), request, false, ios::trunc);
		return isp;
	}

	map<string,string> request_parser(string request) {
		map<string, string> extracted_request;
		vector<string> request_extract = parsers::comma_seperate( request, 0, true);
		string message, number;
		for(auto r_entity : request_extract ) {
			vector<string> component = parsers::equal_seperate( r_entity, 1);
			if(component.size() < 2 ) continue;
			if( component[0] == "number" ) extracted_request.insert(make_pair("number", component[1]));
			else if(component[0] == "message" ) {
				//Remove leading quotes
				if(component[1][0] == '"') component[1].erase(0,1);
				//Remove trailing quotes
				if(component[1][component[1].size()-1] == '"') component[1].erase(component[1].size()-1,1);

				message = component[1];
				extracted_request.insert(make_pair("message", message));
			}
			else if(component[0] == "id") extracted_request.insert(make_pair("id", component[1]));
		}

		return extracted_request;
	}

	void request_distribution_listener( map<string, string> configs ) {
		if( !configs_check(configs)) {
			logger::logger( __FUNCTION__, "Configuration check failed, cannot start", "stderr", true);
			return;
		}

		string PATH_REQUEST_FILE = configs["DIR_REQUEST_FILE"] + "/" + configs["STD_NAME_REQUEST_FILE"];
		while( 1 ) {
			if( helpers::file_exist( PATH_REQUEST_FILE ) ) {
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
						string id = extracted_request["id"];
						isp_distributor( message, number, id, configs );
					}
					if(!sys_calls::file_handlers( random_name, sys_calls::DEL )) {
						logger::logger(__FUNCTION__, "Failed to delete old file, not bad but not good...", "stderr", true);
						logger::logger_errno(errno);
					}
				}
			}
			else {
				logger::logger(__FUNCTION__, "No current request...");
			}

			helpers::sleep_thread( 10 ); 

			//IDEA: could linux inotify replace sleeping the thread; use events to determine when to run
		}
		
	}

}

