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
	string std_response_header = "USSD session initiated; new reply from network:";
	
	map<string,string> configs;
	public:

	USSD( map<string,string> configs );
	USSD( string modem_index, map<string,string> configs );
	USSD() {}
	
	string respond( string );
	string status();
	
	multimap<string,string> initiate_series( vector<string>);
	multimap<string,string> initiate_series( string);
	multimap<string,string> initiate_series( vector<string>, vector<string>);

	bool cancel();

	template<typename TEMPLATED_RETURN_TYPE>
	TEMPLATED_RETURN_TYPE initiate( string );

	void set_ussd_configs( map<string, string> );
};

#endif
