#ifndef MODEM_LISTENER_H_INCLUDED_
#define MODEM_LISTENER_H_INCLUDED_
#include <map>
#include <vector>
#include <mutex>
#include <signal.h>

#include "modem.hpp"
#include "../formatters/helpers.hpp"
#include "../sys_calls/sys_calls.hpp"
using namespace std;


class Modems {
	vector<Modem*> modemCollection;

	map<Modem*, std::thread> threaded_modems;
	int modem_sleep_time = 10;
	int modem_exhaust_count = 3;
	
	MySQL mysqlConnection;
	map<string,string> configs;

	static void handle_sigint( int );

	public:
		static map<string, Modem*> available_modems;
		enum STATE{TEST, PRODUCTION};
		STATE state;
		Modems( map<string,string> configs, STATE state);
		Modems();

		void daemon( bool request_listening, bool sms_listening, bool remote_control);
		void startAllModems();
		void set_modem_sleep_time( int );
		void set_exhaust_count( int );

		vector<string> getAllIndexes();
		vector<string> getAllISP();
		vector<string> getAllIMEI();

		vector<Modem*> find_modem_type( string modem_isp, string type );
		vector<Modem*> find_modem( string modem_index );

		vector<Modem*> getAllModems();

		map<string,map<string,string>> get_available_modems();
		map<string,string> get_modem_details( map<string, string> modem );

		bool db_insert_modems( map<string,string> modem );
		bool db_switch_power_modems( map<string,string> modem, string state);
		bool db_iterate_modems_workload( map<string,string> modem );
};

#endif
