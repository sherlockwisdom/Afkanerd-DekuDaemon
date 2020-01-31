#include <map>
#include <vector>
#ifndef MODEM_LISTENER_H_INCLUDED_
#define MODEM_LISTENER_H_INCLUDED_
using namespace std;

class Modems {
	public:
		Modems();
		Modems getAll();
		
		vector<string> getAllIndexes();
		vector<string> getAllISP();
		vector<string> getAllIMEI();
		~Modems(){}
};

#endif
