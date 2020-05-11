#ifndef MODEM_LISTENER_H_INCLUDED_
#define MODEM_LISTENER_H_INCLUDED_
#include <map>
#include <vector>
#include "modem.hpp"
#include "../formatters/helpers.hpp"
#include "../sys_calls/sys_calls.hpp"
#include <mutex>
#include <signal.h>
using namespace std;


class Modems {
	vector<Modem*> modemCollection;
	static map<string, Modem*> available_modems;

	map<Modem*, std::thread> threaded_modems;
	int modem_sleep_time = 10;
	int modem_exhaust_count = 3;
	
	MySQL mysqlConnection;
	map<string,string> configs;

	static void handle_sigint( int );

	public:
		enum STATE{TEST, PRODUCTION};
		STATE state;
		Modems( map<string,string> configs, STATE state);
		Modems();

		void begin_scanning( bool request_listening, bool sms_listening);
		void startAllModems();
		void set_modem_sleep_time( int );
		void set_exhaust_count( int );
		void db_insert_modems_workload( map<string,string> modem );
		void db_insert_modems( map<string,string> modem );
		void db_switch_power_modems( map<string,string> modem, string state);
		
		vector<string> getAllIndexes();
		vector<string> getAllISP();
		vector<string> getAllIMEI();

		vector<Modem*> find_modem_type( string modem_isp );

		vector<Modem*> getAllModems();

		map<string,map<string,string>> get_available_modems();
		map<string,string> get_modem_details( map<string, string> modem );
};

#endif
