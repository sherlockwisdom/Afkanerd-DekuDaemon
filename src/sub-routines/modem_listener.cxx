#include "modem_listener.hpp"
#include "../formatters/helpers.hpp"
#include "../sys_calls/sys_calls.hpp"

using namespace std;

//class Modem
Modem::Modem() {}

void Modem::setIMEI( string IMEI ) {}
void Modem::setISP( string ISP ) {}
void Modem::setIndex( string index ) {
	this->index = index;
}

string Modem::getIndex() {
	return this->index.empty() ? "" : this->index;
}

//class Modems
Modems::Modems() {}

void Modems::__INIT__() {
	Modems modems;
	string list_of_modem_indexes = sys_calls::terminal_stdout("../../scripts/modem_information_extraction.sh list");
	vector<string> modem_indexes = helpers::split(list_of_modem_indexes, '\n', true);
	
	for(auto index : modem_indexes) {
		string modem_information = sys_calls::terminal_stdout("../../scripts/modem_information_extraction.sh extract " + index );
		vector<string> ln_modem_information = helpers::split(modem_information, '\n', true);

		Modem modem;
		modem.setIndex( index );
		for(auto ln : ln_modem_information) {
			vector<string> component = helpers::split(ln, ':', true);
			if( component[0] == "equipment_id") 
				modem.setIMEI( component[1].empty() ? "" : component[1] );
			else 
			if( component[0] == "operator_name")
				modem.setISP( component[1].empty() ? "" : component[1]);
			else
			if( component[0] == "signal_quality") {}

		}
		this->modemCollection.push_back( modem );
	}
}
vector<string> Modems::getAllIndexes() {
	vector<string> list;
	for(auto modem : this->modemCollection ) {
		list.push_back( modem.getIndex() );
	}
	return list;
}

vector<string> Modems::getAllISP() {}

vector<string> Modems::getAllIMEI() {}
