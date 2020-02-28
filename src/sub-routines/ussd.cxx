#include "ussd.hpp"

using namespace std;

USSD::USSD( map<string,string> configs ) {
	this->configs = configs;
}

USSD::USSD( string modem_index, map<string,string> configs ) {
	this->modem_index = modem_index;
	this->configs = configs;
}

string USSD::initiate( string command ) {
	string terminal_request = this->configs["DIR_SCRIPTS"] + "/modem_information_extraction.sh ussd_initiate " + this->modem_index + " " + command;
	//logger::logger(__FUNCTION__, terminal_request );

	string response = sys_calls::terminal_stdout( terminal_request );
	//logger::logger(__FUNCTION__, response);
	return response;
}

map<size_t,string> USSD::initiate_series( vector<string> commands ) {
	map<size_t, string> responses;

	if( commands.empty() ) return responses;
	
	size_t commands_at = 0;
	string terminal_response = this->initiate( commands[ commands_at ] );
	
	++commands_at;
	if( terminal_response.empty()) return responses;
	responses.insert(make_pair(commands_at, terminal_response));
	for(auto command : commands ) {
		terminal_response = this->respond( command );
		if( terminal_response.empty()) break;
		responses.insert(make_pair(commands_at, terminal_response));
		++commands_at;
	}

	return responses;
}

string USSD::respond( string command ) {}

string USSD::status() {}

bool USSD::cancel() {}
