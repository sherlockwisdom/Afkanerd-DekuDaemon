#include "modem_listener.hpp"
#include "../formatters/helpers.hpp"
#include "../sys_calls/sys_calls.hpp"
#include <mutex>

using namespace std;

//class Modems
Modems::Modems( STATE state) {
	this->state = state;
	switch( state ) {
		case TEST:
			logger::show_state = "TESTING";
		break;

		case PRODUCTION:
			logger::show_state = "PRODUCTION";
		break;
	}
}

void Modems::__INIT__( map<string, string> configs ) {

	while( 1 ) {
		logger::logger(__FUNCTION__, "Refreshing modem list..");
		string list_of_modem_indexes = sys_calls::terminal_stdout(configs["DIR_SCRIPTS"] + "/modem_information_extraction.sh list");
		//logger::logger(__FUNCTION__, list_of_modem_indexes );
		vector<string> modem_indexes = helpers::split(list_of_modem_indexes, '\n', true);
		
		for(auto index : modem_indexes) {
			//logger::logger(__FUNCTION__, "working with index: " + index );
			index = helpers::remove_char( index, ' ', 'E');
			string modem_information = sys_calls::terminal_stdout(configs["DIR_SCRIPTS"] + "/modem_information_extraction.sh extract " + index );
			vector<string> ln_modem_information = helpers::split(modem_information, '\n', true);

			Modem::STATE modem_state = Modem::TEST;
			if(this->state == TEST) modem_state = Modem::TEST; 
			else if(this->state == PRODUCTION) modem_state = Modem::PRODUCTION;
			
			Modem modem( configs, modem_state);
			modem.setIndex( index );
			for(auto ln : ln_modem_information) {
				//logger::logger(__FUNCTION__, "line: " + ln);
				vector<string> component = helpers::split(ln, ':', true);
				if((component.size() != 2 or component[1].empty())) {
					logger::logger(__FUNCTION__, "Incomplete data for modem at index: " + index, "stderr");
					continue;
				}
				else if(component[0] == "equipment_id") modem.setIMEI( component[1]);
				else if(component[0] == "operator_name") {
					//logger::logger(__FUNCTION__, "Setting ISP: " + component[1]);
					modem.setISP( component[1]);
				}
			}
			if(std::find(this->modemCollection.begin(), this->modemCollection.end(), modem) == this->modemCollection.end()) {
				if(modem ) {
					string modem_info = modem.getIMEI() + "|" + modem.getISP();
					logger::logger(__FUNCTION__, modem_info + " - Adding modem to list");
					this->modemCollection.push_back( modem );
				}
			}
			else {
				logger::logger(__FUNCTION__, "Modem already present...");
			}
		}
		//logger::logger(__FUNCTION__, "Exited..");
		helpers::sleep_thread( 10 );
	}
}
vector<string> Modems::getAllIndexes() {
	vector<string> list;
	for(auto modem : this->modemCollection ) {
		list.push_back( modem.getIndex() );
	}
	return list;
}

vector<string> Modems::getAllISP() {
	vector<string> list;
	for(auto modem : this->modemCollection) {
		list.push_back( modem.getISP() );
	}
	return list;
}

vector<string> Modems::getAllIMEI() {
	vector<string> list;
	for(auto modem: this->modemCollection) {
		list.push_back( modem.getIMEI() );
	}
	return list;
}

vector<Modem> Modems::getAllModems() {
	return this->modemCollection;
}

bool Modems::start( Modem modem ) {

}

void Modems::startAllModems() {
	for(auto& modem : this->modemCollection) {
		this->threaded_modems.push_back( std::thread(&Modem::start, modem));
	}

	for(int i=0;i<this->threaded_modems.size();++i){
		this->threaded_modems[i].detach();
	}
}
