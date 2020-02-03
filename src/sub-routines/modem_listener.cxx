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
		logger::logger(__FUNCTION__, "Number of indexes: " + to_string(modem_indexes.size()));
		logger::logger(__FUNCTION__, "Number of collected modems: " + to_string(this->modemCollection.size()));
		for(auto& index : modem_indexes )	
			index = helpers::remove_char( index, ' ', 'E');

		vector<Modem*> tmp_modemCollection;
		for(auto index : modem_indexes) {
			//logger::logger(__FUNCTION__, "working with index: " + index );
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
			//TODO: What happens if a modem changes, but index remains
			//TODO: what happens when a modem is completely removed
			auto it_modemCollection = std::find_if(this->modemCollection.begin(), this->modemCollection.end(), [&](Modem* modem1){ return *modem1 == modem; });
			if(it_modemCollection == this->modemCollection.end()) {
				logger::logger(__FUNCTION__, modem.getInfo() + " - Not found in list");
				if(modem) {
					logger::logger(__FUNCTION__, modem.getInfo() + " - Adding modem to list");
					tmp_modemCollection.push_back( new Modem(modem) );
				}
			}
			else {
				logger::logger(__FUNCTION__, modem.getInfo() + " - Modem already present...");
				//Find and store
				tmp_modemCollection.push_back(*it_modemCollection);
			}
		}
		//logger::logger(__FUNCTION__, "Exited..");
		logger::logger(__FUNCTION__, "Finished refreshing with " + to_string(tmp_modemCollection.size()) + " modems");
		this->modemCollection = tmp_modemCollection;
		helpers::sleep_thread( 10 );
	}
}
vector<string> Modems::getAllIndexes() {
	vector<string> list;
	for(auto modem : this->modemCollection ) {
		list.push_back( modem->getIndex() );
	}
	return list;
}

vector<string> Modems::getAllISP() {
	vector<string> list;
	for(auto modem : this->modemCollection) {
		list.push_back( modem->getISP() );
	}
	return list;
}

vector<string> Modems::getAllIMEI() {
	vector<string> list;
	for(auto modem: this->modemCollection) {
		list.push_back( modem->getIMEI() );
	}
	return list;
}

vector<Modem*> Modems::getAllModems() {
	return this->modemCollection;
}

bool Modems::start( Modem modem ) {

}

void Modems::startAllModems() {
	while( 1 ) {  //TODO: Change this to a condition
		logger::logger(__FUNCTION__, to_string(this->threaded_modems.size()) + " currently threaded modems");
		for(map<Modem*, std::thread>::iterator it=this->threaded_modems.begin();it!=this->threaded_modems.end();++it ) {
			//if(std::find(this->modemCollection.begin(), this->modemCollection.end(), it->first) == this->modemCollection.end()) { //This checks against mem values
			Modem* modem = it->first;
			if(std::find_if(this->modemCollection.begin(), this->modemCollection.end(), [&](Modem* modem1){ return *modem1 == *modem; }) == this->modemCollection.end()) { //This checks against type of modem
				logger::logger(__FUNCTION__, it->first->getInfo() + " - Modem not available, stopping thread");
				if(this->threaded_modems.find(it->first) != this->threaded_modems.end()) {
					it->first->end();
					//while(!it->first->getThreadSafety()) helpers::sleep_thread(5); //TODO: remove this part from the code all together... nothing can kill a detached thread except OS exceptions or the thread dies off
					this->threaded_modems.erase(it);
					if(this->threaded_modems.empty()) {
						logger::logger(__FUNCTION__, "Currently no modem threads running");
						break;
					}
					else {
						if(auto tmp_it = ++it; tmp_it != this->threaded_modems.end()) {
							logger::logger(__FUNCTION__, "Would break the next iteration, exiting loop");
							break;
						}
					}
					logger::logger(__FUNCTION__, to_string(this->threaded_modems.size()) + " currently threaded modems");
				}
			}
		}

		for(auto& modem : this->modemCollection) {
			if(this->threaded_modems.find(modem) == this->threaded_modems.end()) {
				this->threaded_modems.insert(make_pair(modem, std::thread(&Modem::start, modem)));
				logger::logger(__FUNCTION__, modem->getInfo() + " - Began thread...");
				this->threaded_modems[modem].detach();
			}
			else {
				logger::logger(__FUNCTION__, modem->getInfo() + " - Already threaded..." );
			}
		}
		helpers::sleep_thread( 5 );
	}
	
}
