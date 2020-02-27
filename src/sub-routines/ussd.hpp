#ifndef USSD_H_INCLUDED_
#define USSD_H_INCLUDED_

using namespace std;

class USSD {
	public:

	USSD();
	USSD( string);
	
	string initiate( string );
	string respond( string );
	string getStatus();

	bool cancel();
};

#endif
