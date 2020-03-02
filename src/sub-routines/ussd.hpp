#include <iostream>
#include <map>
#include <vector>
#include "../formatters/helpers.hpp"
#include "../sys_calls/sys_calls.hpp"
#ifndef USSD_H_INCLUDED_
#define USSD_H_INCLUDED_

using namespace std;

class USSD {
	private:
	string modem_index;
	
	map<string,string> configs;
	public:

	USSD( map<string,string> );
	USSD( string, map<string,string> );
	
	string initiate( string );
	string respond( string );
	string status();
	
	multimap<string,string> initiate_series( vector<string>);
	multimap<string,string> initiate_series( vector<string>, vector<string>);

	bool cancel();
};

#endif
