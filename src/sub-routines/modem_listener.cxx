#include "modem_listener.hpp"
#include "../formatters/helpers.hpp"
#include "../sys_calls/sys_calls.hpp"

using namespace std;

//class Modem
Modem::Modem() {}

void Modem::setIMEI( string IMEI ) {
	this->imei = IMEI;
}
void Modem::setISP( string ISP ) {
	this->isp = ISP;
}
void Modem::setIndex( string index ) {
	this->index = index;
}

string Modem::getIndex() {
	return this->index.empty() ? "" : this->index;
}

string Modem::getISP() const{
	return this->isp.empty() ? "" : this->isp;
}

string Modem::getIMEI() {
	return this->imei.empty() ? "" : this->imei;
}

Modem::operator bool() const {
	return !this->getISP().empty();
}

bool Modem::start() {
	std::thread tr_modem_request_listener(&Modem::modem_request_listener, this);
	tr_modem_request_listener.join(); //TODO: change to detach
	return false;
}

bool Modem::end() {
	return false;
}

string Modem::getErrorLogs() {
	return this->errorLogs;
}

void Modem::modem_request_listener( ) {
	logger::logger(__FUNCTION__, this->getIMEI() + " thread started...");
	//TODO: https://en.cppreference.com/w/cpp/thread/mutex
}

void Modem::modem_state_listener( Modem modem ) {}

//class Modems
Modems::Modems() {}

void Modems::__INIT__() {
	string list_of_modem_indexes = sys_calls::terminal_stdout("../../scripts/modem_information_extraction.sh list");
	//logger::logger(__FUNCTION__, list_of_modem_indexes );
	vector<string> modem_indexes = helpers::split(list_of_modem_indexes, '\n', true);
	
	for(auto index : modem_indexes) {
		//logger::logger(__FUNCTION__, "working with index: " + index );
		index = helpers::remove_char( index, ' ', 'E');
		string modem_information = sys_calls::terminal_stdout("../../scripts/modem_information_extraction.sh extract " + index );
		vector<string> ln_modem_information = helpers::split(modem_information, '\n', true);

		Modem modem;
		modem.setIndex( index );
		for(auto ln : ln_modem_information) {
			//logger::logger(__FUNCTION__, "line: " + ln);
			vector<string> component = helpers::split(ln, ':', true);
			if((component.size() != 2 or component[1].empty())) {
				logger::logger(__FUNCTION__, "Incomplete data for modem", "stderr");
				continue;
			}
			else if(component[0] == "equipment_id") modem.setIMEI( component[1]);
			else if(component[0] == "operator_name") {
				//logger::logger(__FUNCTION__, "Setting ISP: " + component[1]);
				modem.setISP( component[1]);
			}
		}
		if(modem) {
			//logger::logger(__FUNCTION__, "Adding modem to list");
			this->modemCollection.push_back( modem );
		}
	}
	//logger::logger(__FUNCTION__, "Exited..");
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

bool Modems::start( Modem modem ) {}
