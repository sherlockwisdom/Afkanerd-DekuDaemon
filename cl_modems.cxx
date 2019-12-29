
class DekuFunctions {
	public:
		string request_a_job() {}

		map<string,string> extract_jobs();

}; 

class Modems {
	private:
		void run_daemon_listener();
	public:
		bool is_available( string imei );

};

class Modem {
	private:
		string type;
		string index;
		string imei;
		string isp;
	public:
		Modem(){};

		void start_modem_instance();

		bool send_sms( string message, string number );
};
