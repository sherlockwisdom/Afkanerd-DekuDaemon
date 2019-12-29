
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

void Modems::run_daemon_listener() {

	string func_name = "Modems::run_daemon_listener";

	string str_stdout = helpers::terminal_stdout( this->modem_information_extraction( "list" ));

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
			
			this->modems.push_back( modem_information );
		}
	}
}

string Modems::modem_information_extraction( string arg ) {
	string func_name = "Modems::modem_information_extraction";
	string ex_command = GET_MODEM_INFO();
	ex_command = ex_command + " " + arg;
	logger::logger(func_name, ex_command + "\n");

	return ex_command;
}

void Modem::start_modem_instance() {
	string func_name = "Modem::modem_instance";

	int current_iterate_counter = 0;
	while( this->is_available( this->imei) and current_iterate_counter <= iterate_max ) {
		Job job( this->isp );
		string job_filename = job.request_a_job( isp );

		if( !job.is_job() ) {
			logger::logger( func_name, "no job for modem at this time...");
		}

		map<string,string> ex_job_request = extract_job( job_request );
		string number = ex_job_request["number"];
		string message = ex_job_request["message"];

		if( type == "mmcli" and !message.empty() and !number.empty()) {
			logger::logger( func_name, "sending a mmcli job" );
			if( mmcli_send( message, number, index ) ) {
				logger::logger( func_name, "mmcli message sent successfully...");
			}
			else {
				logger::logger( func_name, "mmcli message failed...", "stderr" );
				helpers::rename_file( job_filename );
			}
		}
		else if ( type == "ssh" and !message.empty() and !number.empty()) {
			logger::logger( func_name, "sending an ssh job" );
			if( ssh_send( message, number, index )) {
				logger::logger( func_name, "ssh message sent successfully...");
			}
			else {
				logger::logger( func_name, "mmcli message failed...", "stderr");
				helpers::unhide_file( job_filename );
			}
		}
		else {
			logger::logger( func_name, "type, message or number is empty.. not good", "stderr", true);
			helpers::delete_file( job_filename );
		}

		helpers::sleep_thread( 10 ); //sleep this n seconds
	}
}
bool Modem::send_sms( string message, string number ) {}
