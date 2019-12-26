#include "helpers.hpp"
#include "declarations.hpp"

//TODO: if modem becomes disconnected, reiterate to find it... meaning, the check for plug and unplug is here


void modem_listener( map<string,string> modem_info ) {
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
