#include <map>
#include <vector>
#include <thread>
#include "../mysql/mysql.hpp"
#ifndef MODEM_H_INCLUDED_
#define MODEM_H_INCLUDED_
using namespace std;

class Modem {
	string index;
	string isp;
	string imei;
	string errorLogs;

	bool keepAlive = false;
	bool thread_safety = false;

	map<string,string> configs;
	
	int failed_counter = 0;
	int sleep_time = 10;
	int modem_exhaust_counts = 3;

	MySQL mysqlConnector;

	public:
		enum STATE {TEST, PRODUCTION};
		enum WORKING_STATE {ACTIVE, EXHAUSTED};
		enum TYPE{SSH, MMCLI};

		STATE state;
		WORKING_STATE working_state;
		Modem(map<string,string> configs, STATE state = TEST );
		Modem(const Modem& modem);
		~Modem();

		void setIndex( string index );
		void setIMEI( string IMEI );
		void setISP( string ISP );
		void setKeepAlive( bool keepAlive);
		void start();
		void setThreadSafety( bool thread_safety );
		void setType( string type );
		void set_modem_exhaust( int );
		void reset_failed_counter();
		void iterate_failed_counter();
		void set_sleep_time( int );

		string getIndex() const;
		string getISP() const;
		string getIMEI() const;
		string getErrorLogs();
		string getInfo() const;

		TYPE getType() const;
		WORKING_STATE db_get_working_state() const;

		explicit operator bool() const;
		bool operator==(Modem modem) const;
		bool operator==(Modem* modem) const;
		bool operator>(Modem modem) const;
		bool operator<(Modem modem) const;

		bool end();
		bool send_sms(string message, string number);
		bool mmcli_send_sms(string message, string number);
		bool ssh_send_sms(string message, string number);
		bool db_set_working_state( WORKING_STATE );
		bool getKeepAlive() const;
		bool getThreadSafety() const;

		map<string,string> request_job( string path_dir_request );
		map<string,string> getConfigs() const;
		map<string, string> get_sms_message( string modem_index ) const;

		vector<map<string,string>> get_sms_messages() const;

		int get_failed_counter() const;
		int get_sleep_time() const;
	private:
		TYPE type;
};

#endif
