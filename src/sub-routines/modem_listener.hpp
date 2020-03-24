#include <map>
#include <vector>
#include "modem.cxx"
#ifndef MODEM_LISTENER_H_INCLUDED_
#define MODEM_LISTENER_H_INCLUDED_
using namespace std;


class Modems {
	vector<Modem*> modemCollection;
	map<Modem*, std::thread> threaded_modems;
	int modem_sleep_time = 10;
	int modem_exhaust_count = 3;
	public:
		enum STATE{TEST, PRODUCTION};
		STATE state;
		Modems( STATE state);
		Modems();

		void __INIT__( map<string,string> configs );
		void startAllModems();
		void set_modem_sleep_time( int );
		void set_exhaust_count( int );
		
		vector<string> getAllIndexes();
		vector<string> getAllISP();
		vector<string> getAllIMEI();

		vector<Modem*> getAllModems();
};

#endif
