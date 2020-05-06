#include <iostream>
#include <map>
#include <vector>
#include "../formatters/helpers.hpp"
#include "../sys_calls/sys_calls.hpp"
#ifndef USSD_H_INCLUDED_
#define USSD_H_INCLUDED_

using namespace std;

class USSD {
	protected:
	string modem_index;
	string response;
	string reply;
	string std_response_header = "USSD session initiated; new reply from network:";
	
	map<string,string> configs;
	multimap<string,string> responses;
	public:

	USSD( map<string,string> configs );
	USSD( string modem_index, map<string,string> configs );
	USSD() {}
	
	string status();
	string get_response() const;
	string get_reply() const; // Reply from a session (called in series)
	multimap<string,string> get_responses() const;
	

	bool cancel();
	bool initiate( string command );
	bool initiate_series( vector<string>);
	bool respond( string );

	void set_ussd_configs( map<string, string> );
	void reset_state();
};

#endif
