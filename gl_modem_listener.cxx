#include "helpers.hpp"
#include "declarations.hpp"

//TODO: if modem becomes disconnected, reiterate to find it... meaning, the check for plug and unplug is here

bool is_ssh_modem( string ip ) {
	return ip.find( GL_SSH_IP_GATEWAY ) != string::npos;
}


string get_modem_type( string modem_index ) {
	return is_ssh_modem( modem_index ) ? "ssh" : "mmcli";
}


string modem_information_extraction( string arg ) {
	string func_name = "modem_information_extraction";
	string ex_command = GET_MODEM_INFO();
	ex_command = ex_command + " " + arg;
	logger::logger(func_name, ex_command + "\n");

	return ex_command;
}

vector<map<string,string>> gl_modem_listener( ) {
	string func_name = "gl_modem_listener";
	vector<map<string,string>> list_of_modems;

	string str_stdout = helpers::terminal_stdout( modem_information_extraction( "list" ));

	if(str_stdout.empty()) {
		logger::logger(func_name, "No modems found!", "stderr" );
		return list_of_modems;
	}
	else {
		vector<string> modem_indexes = helpers::split( str_stdout, '\n', true);
		for( auto& modem_index : modem_indexes ) {
			modem_index = helpers::remove_char( modem_index, ' ', 'E');
			map<string,string> modem_information = {
				{"index", modem_index},
				{"type", get_modem_type( modem_index )}
			};
			
			list_of_modems.push_back( modem_information );
		}
	}

	return list_of_modems;
}

bool has_modem( string unique_modem_id, vector<map<string,string>> modems ) {
	for( auto modem :  modems ) {
		if( modem.find("imei") != modem.end() and modem["imei"] == unique_modem_id ) return true;
	}
	return false;
}

map<string,string> extract_job( vector<string> job ) {
	string number = job[0];
	string message;

	for( int i=1;i<job.size(); ++i) 
		message += job[i];

	return map<string,string>({{"message", message}, {"number", number}});
}

bool mmcli_send( string message, string number, string index ) {}

bool ssh_send( string message, string number, string index ) {}

string request_a_job( string isp ) {}

bool resume_a_job( string isp, string filename ) {}

bool delete_a_job( string isp, string filename ) {}

void modem_instance( map<string,string> modem_info, vector<map<string,string>>& gl_modems_listing, int iterate_max = 0 ) {
	string func_name = "modem_instance";
	//TODO: Ideas, keep this active looking for modem information and shutdown after some tries

	string index = modem_info["index"];
	string imei = modem_info["imei"];
	string isp = modem_info["isp"];
	string type = modem_info["type"];

	int current_iterate_counter = 0;
	while( has_modem( imei, gl_modems_listing ) and current_iterate_counter <= iterate_max ) {
		string job_filename = request_a_job( isp );

		if( job_filename.empty() or !helpers::file_exist( job_filename ) ) {
			logger::logger( func_name, "no job for modem at this time...");
		}

		vector<string> job_request = helpers::read_file( job_filename );
		
		if( job_request.size() > 1 ) {
			map<string,string> ex_job_request = extract_job( job_request );
			string number = ex_job_request["number"];
			string message = ex_job_request["message"];

			if( type == "mmcli" and !message.empty() and !number.empty()) {
				logger::logger( func_name, "sending a mmcli job" );
				if( mmcli_send( message, number, index ) ) {
					logger::logger( func_name, "mmcli message sent successfully...");
				}
				else {
					logger::logger( func_name, "mmcli message failed...", "stderr" );
					resume_a_job( isp, job_filename );
				}
			}
			else if ( type == "ssh" and !message.empty() and !number.empty()) {
				logger::logger( func_name, "sending an ssh job" );
				if( ssh_send( message, number, index )) {
					logger::logger( func_name, "ssh message sent successfully...");
				}
				else {
					logger::logger( func_name, "mmcli message failed...", "stderr");
					resume_a_job( isp, job_filename );
				}
			}
			else {
				logger::logger( func_name, "type, message or number is empty.. not good", "stderr", true);
				delete_a_job( isp, job_filename );
			}
		}
		else {
			logger::logger( func_name, "Invalid job file...", "stderr", true );
			delete_a_job( isp, job_filename );
		}
		if( iterate_max > 0) 
			++current_iterate_counter;

		helpers::sleep_thread( 10 ); //sleep this n seconds
	}
}

void daemon_start_modem_instance_listeners() {}
