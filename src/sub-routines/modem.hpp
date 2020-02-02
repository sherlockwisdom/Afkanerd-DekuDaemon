#include <thread>
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
	
	public:
		enum STATE {TEST, PRODUCTION};
		STATE state;
		Modem(map<string,string> configs, STATE state = TEST);
		~Modem();

		void setIndex( string index );
		void setIMEI( string IMEI );
		void setISP( string ISP );
		//void modem_request_listener(map<string,string> configs={});
		//void modem_state_listener( );
		void setKeepAlive( bool keepAlive);
		void start();
		void setThreadSafety( bool thread_safety );

		string getIndex() const;
		string getISP() const;
		string getIMEI() const;
		string getErrorLogs();

		explicit operator bool() const;
		bool operator==(Modem modem) const;

		bool end();
		bool send_sms(string message, string number);
		bool getKeepAlive();
		bool getThreadSafety();

		map<string,string> request_job( string path_dir_request );
		map<string,string> getConfigs();
};

#endif
