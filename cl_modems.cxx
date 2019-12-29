
class Jobs {
	public:
		string request_a_job();

		map<string,string> extract_jobs();

		void set_isp( string isp );
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


string Jobs::request_a_job( ) {}
map<string,string> Jobs::extract_jobs() {}
void Jobs::set_isp( string isp ) {}



bool Modems::is_available() {}
string Modems::get_modem_type() {}

bool Modems::is_ssh_modem( string index ) {}
bool Modems::run_daemon_listener() {}

string Modems::modem_information_extraction( string arg ) {}

void Modem::start_modem_instance() {}
bool Modem::send_sms( string message, string number ) {}
