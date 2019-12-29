
class Jobs {
	public:
		string request_a_job() {}

		map<string,string> extract_jobs();
}; 

class Modems {
	protected: 
		string modem_information_extraction( string arg );
	private:
		void run_daemon_listener();
		bool is_ssh_modem( string index );
	public:
		bool is_available( string imei );
		string get_modem_type( string index );

};

class Modem : public Modems, Jobs {
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
