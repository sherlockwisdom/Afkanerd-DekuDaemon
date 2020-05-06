#include "ussd.hpp"
#include <type_traits>

using namespace std;

USSD::USSD( map<string,string> configs ) {
	this->configs = configs;
}

USSD::USSD( string modem_index, map<string,string> configs ) {
	this->modem_index = modem_index;
	this->configs = configs;
}

void USSD::set_ussd_configs( map<string,string> configs ) {
	this->configs = configs;
}

string USSD::get_response() const {
	return this->response;
}

string USSD::get_reply() const {
	return this->reply;
}

multimap<string,string> USSD::get_responses() const {
	return this->responses;
}

void USSD::reset_state() {
	this->response = "";
	this->responses.clear();
}

bool USSD::initiate( string command ) {
	this->reset_state(); // Else if it fails, it will provide the last successful reply
	bool state = false;

	string terminal_request = this->configs["DIR_SCRIPTS"] + "/modem_information_extraction.sh ussd_initiate " + this->modem_index + " " + command;

	logger::logger(__FUNCTION__, terminal_request );
	string response = sys_calls::terminal_stdout( terminal_request );
	if( response.empty()) return state;

	this->response = response;

	// Doing this using strict methods of extracting the exact match of what the response should be
	string std_header_loc = response.substr(0, std_response_header.size());
	// logger::logger(__FUNCTION__, std_header_loc);
	// state = response.find( this->std_response_header ) != string::npos ? true : false;
	state = std_header_loc == std_response_header;
	return state;
}

bool USSD::initiate_series( vector<string> commands ) {
	this->reset_state();
	bool state = false;

	bool command_state = this->initiate( commands[0] );
	if( commands.size() == 1 )
		return command_state;

	string terminal_response = this->get_response();
	if( terminal_response.empty()) return state;
	this->response = terminal_response;
	logger::logger(__FUNCTION__, response );

	this->responses.insert(make_pair(commands[0], terminal_response));
	for(auto command = commands.begin() + 1; command != commands.end(); ++command) {
		state = false;
		bool respond_state = this->respond( *command );
		logger::logger(__FUNCTION__, this->get_reply());
		if( !respond_state ) break;

		this->responses.insert(make_pair(*command, this->get_reply()));
		state = true;
	}

	return state;
}

bool USSD::respond( string command ) {
	// this->reset_state();
	bool state = false;
	string terminal_request = this->configs["DIR_SCRIPTS"] + "/modem_information_extraction.sh ussd_respond " + this->modem_index + " " + command;
	//logger::logger(__FUNCTION__, terminal_request );

	this->reply = sys_calls::terminal_stdout( terminal_request );
	string is_header = this->reply.substr(0, std_response_header.size());
	state = is_header == std_response_header;
	return state;
}

string USSD::status() {
	string terminal_request = this->configs["DIR_SCRIPTS"] + "/modem_information_extraction.sh ussd_status " + this->modem_index;
	//logger::logger(__FUNCTION__, terminal_request );

	string response = sys_calls::terminal_stdout( terminal_request );
	// Parse response to get output
	//logger::logger(__FUNCTION__, response);
	//return response;
}

bool USSD::cancel() {
	string terminal_request = this->configs["DIR_SCRIPTS"] + "/modem_information_extraction.sh ussd_cancel " + this->modem_index;
	//logger::logger(__FUNCTION__, terminal_request );

	string response = sys_calls::terminal_stdout( terminal_request );
	// Parse response to get output
	//logger::logger(__FUNCTION__, response);
	//return response;
}
