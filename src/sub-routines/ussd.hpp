#include <iostream>
#include <map>
#include <vector>
#ifndef USSD_H_INCLUDED_
#define USSD_H_INCLUDED_

using namespace std;

class USSD {
	public:

	USSD();
	USSD( string );
	
	string initiate( string );
	string respond( string );
	string status();
	
	map<string,string> initiate_series( vector<string>);

	bool cancel();
};

#endif