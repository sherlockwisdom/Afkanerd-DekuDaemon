#include <map>
#include <vector>
#include "modem.cxx"
#ifndef MODEM_LISTENER_H_INCLUDED_
#define MODEM_LISTENER_H_INCLUDED_
using namespace std;


class Modems {
	vector<Modem> modemCollection;
	public:
		Modems();
		void __INIT__();
		
		vector<string> getAllIndexes();
		vector<string> getAllISP();
		vector<string> getAllIMEI();

		vector<Modem> getAllModems();

		bool start(Modem);
};

#endif
