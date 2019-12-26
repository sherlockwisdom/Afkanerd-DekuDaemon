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
	logger::logger( func_name, str_stdout + "\n" );

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

void modem_instance( map<string,string> modem_info ) {
	//TODO: Ideas, keep this active looking for modem information and shutdown after some tries
	
}


void daemon_start_modem_instance_listeners() {}
