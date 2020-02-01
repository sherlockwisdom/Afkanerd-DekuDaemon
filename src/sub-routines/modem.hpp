#ifndef MODEM_H_INCLUDED_
#define MODEM_H_INCLUDED_
using namespace std;

class Modem {
	string index;
	string isp;
	string imei;
	string errorLogs;

	bool keepAlive = false;
	map<string,string> configs;
	public:
		Modem(map<string,string> configs);

		void setIndex( string index );
		void setIMEI( string IMEI );
		void setISP( string ISP );
		void modem_request_listener();
		void modem_state_listener( );

		string getIndex();
		string getISP() const;
		string getIMEI();
		string getErrorLogs();
		string start();

		explicit operator bool() const;

		bool end();
		bool send_sms(string message, string number);

		map<string,string> request_job( string path_dir_request );
};

#endif
