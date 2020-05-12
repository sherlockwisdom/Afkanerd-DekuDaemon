#ifndef MODEM_H_INCLUDED_
#define MODEM_H_INCLUDED_
#include <map>
#include <vector>
#include <thread>
#include <iostream>
#include <mutex>

#include "ussd.hpp"
#include "../mysql/mysql.hpp"
using namespace std;

class Modem : public USSD {
	string index;
	string isp;
	string imei;
	string errorLogs;
	string type;

	bool available = true;

	map<string,string> configs;
	
	int failed_counter = 0;
	int sleep_time = 10;
	int exhaust_count = 7;

	MySQL mysqlConnection;

	public:
		enum STATE {TEST, PRODUCTION};
		enum WORKING_STATE {ACTIVE, EXHAUSTED};
		enum TYPE{SSH, MMCLI};

		STATE state;
		WORKING_STATE working_state;
		Modem(string imei, string isp, string type, string index, map<string, string> configs, MySQL mysqlConnection );
		Modem(string imei, string isp, string type, string index, map<string, string> configs );
		Modem(const Modem& modem);
		Modem() {}
		~Modem();

		void setIndex( string index );
		void setIMEI( string IMEI );
		void setISP( string ISP );
		void setKeepAlive( bool keepAlive);
		void start();
		void setThreadSafety( bool thread_safety );
		void setType( string type );
		void set_exhaust_count( int );
		void reset_failed_counter();
		void iterate_failed_counter();
		void db_iterate_workload();
		void set_sleep_time( int );
		void request_listener();
		void modem_sms_listener();
		void set_configs( map<string,string> configs );
		void set_mysql_connection( MySQL );
		void db_reset_workload();
		void declare_pending( string filename );

		string getIndex() const;
		string getISP() const;
		string getIMEI() const;
		string getErrorLogs();
		string getInfo() const;
		string getType() const;

		WORKING_STATE db_get_working_state() const;

		operator bool() const;
		bool operator==(Modem modem) const;
		bool operator==(Modem* modem) const;
		bool operator>(Modem modem) const;
		bool operator<(Modem modem) const;

		string send_sms(string message, string number);
		string mmcli_send_sms(string message, string number);
		string ssh_send_sms(string message, string number);
		bool db_set_working_state( WORKING_STATE );
		bool getKeepAlive() const;
		bool getThreadSafety() const;
		bool delete_sms( string message_index );
		bool is_available() const;
		bool release_request_file( string locked_filename );

		map<string,string> request_job( string path_dir_request );
		map<string,string> getConfigs() const;
		map<string, string> get_sms_message( string modem_index ) const;

		vector<map<string,string>> get_sms_messages() const;
		vector<string> release_pending_files();

		int get_failed_counter() const;
		int get_sleep_time() const;
		int get_exhaust_count() const;
		int db_get_workload();

		MySQL get_mysql_connector() const;
};

#endif
