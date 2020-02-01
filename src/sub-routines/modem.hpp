#ifndef MODEM_H_INCLUDED_
#define MODEM_H_INCLUDED_
using namespace std;

class Modem {
	string index;
	string isp;
	string imei;
	string errorLogs;

	bool keepAlive = false;
	public:
		Modem();

		void setIndex( string index );
		void setIMEI( string IMEI );
		void setISP( string ISP );
		void modem_request_listener();
		void modem_state_listener( );

		string getIndex();
		string getISP() const;
		string getIMEI();
		string getErrorLogs();

		explicit operator bool() const;

		string start();
		bool end();
};

#endif
