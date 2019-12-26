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
	/*
	//printf("%s=> modem information... [%s]\n", func_name.c_str(), modem_information[2].c_str());
	//XXX: Check if another an instance of the modem is already running
	if(MODEM_DAEMON.find(modem_info["imei"]) != MODEM_DAEMON.end()) {
		cout << func_name << "=> Instance of Modem already running... watch dog reset!" << endl;
		std::this_thread::sleep_for(std::chrono::seconds(GL_TR_SLEEP_TIME));
		return;
	}
	if(modem_service_provider.empty()) 
		modem_service_provider = helpers::split(modem_information[2], ':', true)[1]; //FIXME: What happens when cannot get ISP

	printf("%s=> +ISP[%s] +index[%s] - ", func_name.c_str(), modem_service_provider.c_str(), modem_index.c_str());
	if(mkdir((char*)(SYS_FOLDER_MODEMS + "/" + modem_imei).c_str(), STD_DIR_MODE) != 0 && errno != EEXIST) {
		char str_error[256];
		cerr << "FAILED\n" << func_name << ".error=> " << strerror_r(errno, str_error, 256) << endl;
	}
	else {
		std::thread tr_modem_listener(modem_listener, "\tModem Listener", modem_imei, modem_index, modem_service_provider, true, "MMCLI");
		tr_modem_listener.detach();
	}
	*/
}

/*
void daemon_start_modem_listener() {
	vector<map<string,string>> list_of_modems = gl_modem_listener();
	
}
*/
