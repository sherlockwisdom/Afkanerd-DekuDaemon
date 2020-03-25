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

template<typename TEMPLATED_RETURN_TYPE>
TEMPLATED_RETURN_TYPE USSD::initiate( string command ) {
	string condition;
	TEMPLATED_RETURN_TYPE _return;

	// TODO: Finish working on this, would aid a lot
	if( command.find("{") != string::npos and command.find("}") != string::npos)  {
		logger::logger(__FUNCTION__, "Initiating conditional USSD");
		vector<string> conditions = helpers::split( command, '{', true );
		// This assumes not space at the end, but since I don't read comments, I'd most probably forget
		condition = conditions[1];
		command = conditions[0];
	}

	string terminal_request = this->configs["DIR_SCRIPTS"] + "/modem_information_extraction.sh ussd_initiate " + this->modem_index + " " + command;
	//logger::logger(__FUNCTION__, terminal_request );

	string response = sys_calls::terminal_stdout( terminal_request );

	//Checking if condition is requested, and that's based on the return type
	if( std::is_same_v<TEMPLATED_RETURN_TYPE, bool> ) {
		_return = response.find(condition) != string::npos ? true : false;
	}
	else {
		_return = response;
	}

	//logger::logger(__FUNCTION__, response);
	return response;
}

multimap<string,string> USSD::initiate_series( vector<string> commands ) {
	multimap<string, string> responses;

	if( commands.empty() ) return responses;
	
	string terminal_response = this->initiate<string>( commands[0] );
	if( terminal_response.empty()) return responses;

	responses.insert(make_pair(commands[0], terminal_response));
	for(auto command = commands.begin() + 1; command != commands.end(); ++command) {
		terminal_response = this->respond( *command );
		if( terminal_response.empty()) break;
		responses.insert(make_pair(*command, terminal_response));
	}

	return responses;
}

multimap<string,string> USSD::initiate_series( vector<string> args, vector<string> commands ) {
	multimap<string, string> responses;

	if( args.empty()) return responses;
	if( commands.empty() ) return responses;

	size_t args_counter = 0;
	for( auto& command : commands ) {
		if( command == "%%" ) {
			command = args[args_counter];
			++args_counter;
		}
	}
	
	string terminal_response = this->initiate<string>( commands[0] );
	if( terminal_response.empty()) return responses;

	responses.insert(make_pair(commands[0], terminal_response));
	for(auto command = commands.begin() + 1; command != commands.end(); ++command) {
		terminal_response = this->respond( *command );
		if( terminal_response.empty()) break;
		responses.insert(make_pair(*command, terminal_response));
	}

	return responses;
}

string USSD::respond( string command ) {
	string terminal_request = this->configs["DIR_SCRIPTS"] + "/modem_information_extraction.sh ussd_respond " + this->modem_index + " " + command;
	//logger::logger(__FUNCTION__, terminal_request );

	string response = sys_calls::terminal_stdout( terminal_request );
	//logger::logger(__FUNCTION__, response);
	return response;
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
