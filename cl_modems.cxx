
class Jobs {
	public:
		string request_a_job();

		map<string,string> extract_jobs();

		void set_isp( string isp );
}; 

class Modems {
	protected: 
		string modem_information_extraction( string arg );
		vector<map<string,string>> modems;
	private:
		void run_daemon_listener();
		bool is_ssh_modem( string ip );
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


void Jobs::request_a_job( ) {
	string func_name = "Jobs::request_a_job";
	string path_to_request_file = "";

	string files_in_dir = helpers::terminal_stdout( "ls -1 " + this->path_to_jobs );
	if( !files_in_dir.empty()) {
		vector<string> files = helpers::split( files_in_dir, '\n', true );
		path_to_request_file = this->path_to_jobs + "/" + files[0];
		helpers::hide_file( path_to_request_file );
		this->path_to_request_file;
	}
	else {
		logger::logger( func_name, "No request file" );
	}
	this->pending_job = path_to_request_file;
}

map<string,string> Jobs::extract_jobs() {
	string job = this->pending_job;
	vector<string> job_content = helpers::read_file( job );
	string number = job_content[0];
	string message = "";
	for( int i=1;i<job_content.size();++i)
		message += job_content[i];
	map<string,string> request = { {"number",number}, {"message", message}};
	return request;
}
void Jobs::set_isp( string isp ) {
	this->isp = isp;
}



bool Modems::is_available( string imei ) {
	for( auto modem :  this->modems ) {
		if( modem.find("imei") != modem.end() and modem["imei"] == unique_modem_id ) return true;
	}
	return false;
}

string Modems::get_modem_type( string index ) {
	return this->is_ssh_modem( index ) ? "ssh" : "mmcli";
}

bool Modems::is_ssh_modem( string ip ) {
	return ip.find( GL_SSH_IP_GATEWAY ) != string::npos;
}

bool Modems::run_daemon_listener() {

	string func_name = "Modems::run_daemon_listener";
	vector<map<string,string>> list_of_modems;

	string str_stdout = helpers::terminal_stdout( modem_information_extraction( "list" ));

	if(str_stdout.empty()) {
		logger::logger(func_name, "No modems found!", "stderr" );
		return list_of_modems;
	}
	else {
		vector<string> modem_indexes = helpers::split( str_stdout, '\n', true);
		for( auto& modem_index : modem_indexes ) {
			modem_index = helpers::remove_char( modem_index, ' ', 'E');
			map<string,string> modem_information = {
				{"index", modem_index},
				{"type", get_modem_type( modem_index )}
			};
			
			list_of_modems.push_back( modem_information );
		}
	}

	return list_of_modems;
}

string Modems::modem_information_extraction( string arg ) {}

void Modem::start_modem_instance() {}
bool Modem::send_sms( string message, string number ) {}
