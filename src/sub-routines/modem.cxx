#include "modem.hpp"
#include "saitama.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include "../mysql/mysql.cxx"
#include "../sys_calls/sys_calls.hpp"
#include "request_distribution_listener.cxx"
using namespace std;
std::mutex blocking_mutex;

//class Modem
//TODO: Make notifications when required config variables are missing from program e.g DIR_SCRIPTS typo'd as DIR_SCRIPT
Modem::Modem(string imei, string isp, string type, string index, map<string,string> configs, MySQL mysqlConnection) {
	this->imei = imei;
	this->isp = isp;
	this->type = type;
	this->index = index; 
	this->configs = configs;
	this->mysqlConnection = mysqlConnection;

	this->set_ussd_configs( this->configs );
	this->modem_index = this->index; // Controls the USSD part of the code
}

Modem::Modem(string imei, string isp, string type, string index, map<string,string> configs) {
	this->imei = imei;
	this->isp = isp;
	this->type = type;
	this->index = index; 
	this->configs = configs;

	this->set_ussd_configs( this->configs );
	this->modem_index = this->index; // Controls the USSD part of the code
}

int Modem::get_failed_counter() const {
	return this->failed_counter;
}

void Modem::set_mysql_connection( MySQL mysqlConnection ) {
	this->mysqlConnection = mysqlConnection;
}

void Modem::set_exhaust_count( int modem_exhaust_counts ) {
	logger::logger(__FUNCTION__, this->getInfo() + " - Setting Exhaust count to: " + to_string( modem_exhaust_counts ));
	this->exhaust_count = modem_exhaust_counts;
}

void Modem::setIndex( string index ) {
	this->index = index;
}

void Modem::setIMEI( string IMEI ) {
	this->imei = IMEI;
}

void Modem::set_configs( map<string,string> configs ) {
	this->configs = configs;
}

string Modem::getIndex() const {
	return this->index.empty() ? "" : this->index;
}

string Modem::getISP() const{
	return this->isp.empty() ? "" : helpers::to_uppercase(this->isp);
}

string Modem::getIMEI() const {
	return this->imei.empty() ? "" : this->imei;
}

string Modem::getInfo() const {
	return this->getIMEI() + "|" + this->getISP() + "|" + this->getIndex() + "|" + this->getType();
}

string Modem::getType() const {
	return this->type;
}

bool Modem::delete_sms( string message_index ) {
	string terminal_respond = sys_calls::terminal_stdout( this->getConfigs()["DIR_SCRIPTS"] + "/modem_information_extraction.sh sms delete " + message_index + " " + this->getIndex() );	
	
	logger::logger(__FUNCTION__, terminal_respond );

	return !terminal_respond.empty();
}

map<string,string> Modem::getConfigs() const {
	return this->configs;
}

map<string,string> Modem::get_sms_message( string message_index ) const {
	string terminal_respond = sys_calls::terminal_stdout( this->getConfigs()["DIR_SCRIPTS"] + "/modem_information_extraction.sh sms read_sms " + message_index + " " + this->getIndex() );	
	
	vector<string> message_body = helpers::string_split( terminal_respond, '\n');
	//TODO: if less than 3... somethings wrong
	/*
	for( auto message_line : message_body ) {
		// logger::logger(__FUNCTION__, message_line);
	}
	*/

	string number = message_body[0];
	string message = message_body[1];
	string timestamp = message_body[2];

	return map<string,string> {
		{"number", number},
		{"message", message},
		{"timestamp", timestamp},
		{"index", message_index}
	};
}

Modem::operator bool() const {
	return 
		!this->imei.empty() and
		!this->isp.empty() and
		!this->type.empty() and
		!this->index.empty();
}

bool Modem::operator ==(Modem modem) const {
	return 
		modem.getISP() == this->getISP() and
		modem.getIMEI() == this->getIMEI() and
		modem.getType() == this->getType() and
		modem.getIndex() == this->getIndex();
}

vector<map<string,string>> Modem::get_sms_messages() const {
	vector<map<string,string>> sms_messages;
	string terminal_respond = sys_calls::terminal_stdout( this->getConfigs()["DIR_SCRIPTS"] + "/modem_information_extraction.sh sms received " + this->getIndex() );	
	vector<string> sms_indexes = helpers::string_split( terminal_respond, '\n' );
	logger::logger(__FUNCTION__, "Number of SMS Indexes: " + to_string( sms_indexes.size() ));

	for(auto message_index : sms_indexes) {
		map<string,string> sms_message = this->get_sms_message( message_index );
		// logger::logger(__FUNCTION__, message_index );
		sms_messages.push_back( sms_message );
	}
	logger::logger(__FUNCTION__, "Number of SMS Messages extracted: " + to_string( sms_messages.size() ));

	return sms_messages;
}


//XXX: WORKING HERE ===================>
void Modem::modem_sms_listener ( ) {
	logger::logger(__FUNCTION__, "==========> MODEM SMS LISTENER | " + this->getInfo() + " <============");
	while( 1 ) {
		logger::logger(__FUNCTION__, this->getInfo() + " - Checking for SMS messages");
		vector<map<string,string>> sms_messages = this->get_sms_messages();

		if( !sms_messages.empty()) {
			for( auto sms_message_body : sms_messages ) {
				logger::logger(__FUNCTION__, "=============================");
				string message = sms_message_body["message"];
				string number = sms_message_body["number"];
				string timestamp = sms_message_body["timestamp"];

				logger::logger(__FUNCTION__, "SMS Message: " + message);
				logger::logger(__FUNCTION__, "SMS Number: " + number);
				logger::logger(__FUNCTION__, "SMS Timestamp: " + timestamp );
				logger::logger(__FUNCTION__, "=============================");

				//TODO: put a helper function
				saitama::configs = this->getConfigs();
				saitama::execute( message );

				//TODO: should delete the message once it has been executed - THIS IS VERY URGENT, CUS MODEM INFINITE LOOP
			}
		}
		else {
			logger::logger(__FUNCTION__, this->getInfo() + " - No SMS message found!");
		}
		helpers::sleep_thread( 5 );
	}
}

void Modem::reset_failed_counter() {
	if( this->failed_counter > 0 or !this->working_state ) {
		logger::logger(__FUNCTION__, this->getInfo() + " - Resetting failed counter");
		this->working_state = Modem::ACTIVE;
		this->failed_counter = 0;
	}
}

void Modem::iterate_failed_counter() {
	logger::logger(__FUNCTION__, this->getInfo() + " - Iterating failed counter to: " + to_string( this->failed_counter + 1));
	++this->failed_counter;

	if( this->failed_counter > 3 and this->working_state == Modem::ACTIVE ) { //TODO: make this changeable from systems settings
		logger::logger(__FUNCTION__, this->getInfo() + "- Modem Exhausted Based On Fail Counter!", "stderr", true);
		this->working_state = Modem::EXHAUSTED;
	}
}

Modem::WORKING_STATE Modem::db_get_working_state() const { 
	return this->working_state;
}


void Modem::db_iterate_workload() {
	string query = "UPDATE __DEKU__.MODEM_WORK_LOAD SET WORK_LOAD = WORK_LOAD + 1 WHERE DATE = DATE(NOW()) AND IMEI = '"+this->imei+"'";
	logger::logger(__FUNCTION__, query);

	try {
		map<string, vector<string>> responds = this->mysqlConnection.query( query );
	}
	catch(std::exception& excep) {
		//logger::logger(__FUNCTION__, "Exception says: " + excep.what());
	}
}

bool Modem::db_set_working_state( WORKING_STATE working_state )  {
	this->working_state = working_state;

	string query = "";
	if( working_state == Modem::EXHAUSTED ) 
		query = "UPDATE __DEKU__.MODEMS SET STATE = 'exhausted' WHERE IMEI = '" + this->imei + "'";
	else if( working_state == Modem::ACTIVE ) 
		query = "UPDATE __DEKU__.MODEMS SET STATE = 'active' WHERE IMEI = '" + this->imei + "'";

	logger::logger(__FUNCTION__, query);

	try {
		map<string, vector<string>> responds = this->mysqlConnection.query( query );
	}
	catch(std::exception& excep) {
		//logger::logger(__FUNCTION__, "Exception says: " + excep.what());
	}
}

void Modem::start() {
	std::thread tr_modem_request_listener = std::thread(&Modem::request_listener, this);
	
	//TODO: Checks for incoming sms messages here
	// std::thread tr_modem_sms_listener = std::thread(&Modem::modem_sms_listener, this);

	tr_modem_request_listener.join();
	// tr_modem_sms_listener.join();
}

int Modem::db_get_workload() { // TODO: Should take in date as a variable
	int workload = 0;
	string query = "SELECT * FROM __DEKU__.MODEM_WORK_LOAD WHERE IMEI='"+this->imei+"' AND DATE = DATE(NOW())";
	logger::logger(__FUNCTION__, query);

	try {
		map<string, vector<string>> responds = this->mysqlConnection.query( query );
		if( !responds.empty() and responds.find("WORK_LOAD") != responds.end() and !responds["WORK_LOAD"].empty())
			workload = atoi(responds["WORK_LOAD"][0].c_str());
	}
	catch(std::exception& excep) {
		//logger::logger(__FUNCTION__, "Exception says: " + excep.what());
	}

	return workload;
}

void Modem::db_reset_workload() {
	string query = "UPDATE __DEKU__.MODEM_WORK_LOAD SET WORK_LOAD = 0 WHERE IMEI='"+this->imei+"' AND DATE = DATE(NOW())";
	logger::logger(__FUNCTION__, query);

	try {
		map<string, vector<string>> responds = this->mysqlConnection.query( query );
	}
	catch(std::exception& excep) {
		//logger::logger(__FUNCTION__, "Exception says: " + excep.what());
	}
}


bool Modem::is_available() const {
	vector<string> respond = sys_calls::get_modem_details( this->getConfigs()["DIR_SCRIPTS"], this->index );
	return !respond.empty();
}


vector<string> Modem::release_pending_files() {
	string path_dir_request = this->getConfigs()["DIR_ISP"];
	
	/// Releasing locked file for another modem - ONLY AFTER BEING SURE WASN'T SENT
	if( path_dir_request[path_dir_request.size() - 1] == '/') path_dir_request.erase(path_dir_request.size() -1, 1); // just some cleansing cus don't trust rules are always followed

	map<string,string> ls_returned_values;
	string pending_string_handle = ".PENDING_" + this->getIMEI() + "_";
	sys_calls::terminal_stdout(ls_returned_values, "ls -1 " + path_dir_request + "/" + pending_string_handle + "*" );

	if( atoi(ls_returned_values["return"].c_str()) != 0 ) {
		logger::logger(__FUNCTION__, this->getInfo() + " - NOTHING PENDING FOR RELEASE");
		return vector<string>{};
	}

	vector<string> filenames = helpers::string_split(ls_returned_values["data"], '\n');

	for( auto file : filenames ) {
		string source_file = path_dir_request + "/" + this->getISP() + "/" + file;

		file.erase(0, 1); // Remove the . in front of the filename

		string new_file = path_dir_request + "/" + this->getISP() + "/" + file;

		if( !sys_calls::rename_file( source_file, new_file )) {
			logger::logger(__FUNCTION__, " - FAILED RELEASING PENDING FILES: " + source_file, "stderr");
			logger::logger_errno(errno);
		}
		else {
			logger::logger(__FUNCTION__, " - 200 RELEASED PENDING FILE: " + file, "stdout");
		}
	}
	return filenames;
}


void Modem::declare_pending( string filename ) {
	string new_filename = ".PENDING_" + this->getIMEI() + "_" + filename;
	string full_path = this->getConfigs()["DIR_ISP"] + "/" + this->getISP() + "/";

	filename = full_path + filename;
	new_filename = full_path + new_filename;

	if( !sys_calls::rename_file( filename, new_filename) ) {
		logger::logger(__FUNCTION__, " - FAILED TO DECLARE PENDING: " + filename, "stderr");
	}

	else {
		logger::logger(__FUNCTION__, " - 200 DECLARED PENDING: " + filename, "stdout");
	}
}


// TODO: Remove sms index after messages have been sent
void Modem::request_listener() {
	logger::logger(__FUNCTION__, "==========> MODEM REQUEST LISTENER | " + this->getInfo() + " <============");
	while( 1 ) {
		logger::logger(__FUNCTION__, this->getInfo() + " - Scanning for pending request");
		if(!this->is_available()) {
			logger::logger(__FUNCTION__, this->getInfo() + " | Has gone away |", "stdout", true);
			this->available = false;
			break;
		}

		if(!blocking_mutex.try_lock() ) {
			//logger::logger(__FUNCTION__, this->getInfo() + " - Mutex locked..", "stdout");
			helpers::sleep_thread( 3 );
			continue;
		}

		/// Mutex has been released
		map<string,string> request = this->request_job( this->getConfigs()["DIR_ISP"] + "/" + this->getISP() );
		if( request.empty()) {
			logger::logger(__FUNCTION__, this->getInfo() + " - No request...", "stdout", true);
			blocking_mutex.unlock();

			helpers::sleep_thread(5);
			continue;
		}

		blocking_mutex.unlock();
		logger::logger(__FUNCTION__, this->getInfo() + " - FILE LOCKED: " + request["filename"]);

		string message = helpers::escape_string( request["message"], '"');
		string number = request["number"];
		string number_isp = isp_determiner::get_isp( number );

		/// Checking receiving SMS ISP if it matches ISP of modem
		if( number_isp != this->getISP() ) {
			string move_isp = this->getConfigs()["DIR_ISP"] + "/" + number_isp + "/" + request["q_filename"];
			logger::logger(__FUNCTION__, " - MIGRATING ISP [" + this->getISP() + "] to [" + move_isp + "]", "stderr", true );
			if( !sys_calls::rename_file( request["filename"], move_isp ))
				logger::logger(__FUNCTION__, this->getInfo() + " - Failed to move file to right ISP dir", "stderr", true);
			continue;
		}

		/// Sending SMS message to number
		string send_sms_status = this->send_sms( message, number );
		/*
		- If message is sent, previous messages which have failed can be considered delivered
		- This role applies only in cases where modem has not been declared exhausted
		- 
		- Modems needs to continue in their previous states in other to not overly send messages
		*/
		string full_path_locked_request_filename = request["filename"];
		string open_request_filename = request["q_filename"];
		string locked_request_filename = "." + request["q_filename"];
		string full_path_open_request_filename_success = this->getConfigs()["DIR_SUCCESS"] + "/" + open_request_filename;

		/// SMS sent successfully
		if(  send_sms_status == "done" ) {

			//this->revoke_pending_messages();
			this->reset_failed_counter();

			// this->db_iterate_workload(); // TODO: Allow after running test
			// this->db_set_working_state( Modem::ACTIVE );
			logger::logger(__FUNCTION__, this->getInfo() + " - [" + request["id"] + "] SMS 200", "stdout", true);

			bool opened_request_file = sys_calls::rename_file(full_path_locked_request_filename, full_path_open_request_filename_success);
			if(opened_request_file) {
				logger::logger(__FUNCTION__, this->getInfo() + " - MOVED TO 200", "stdout", true);
			}
			else {
				logger::logger(__FUNCTION__, this->getInfo() + " - FAILED MOVED TO 200", "stderr", true);
				// TODO: Delete file
			}
		}

		/// SMS failed
		else if( send_sms_status == "failed") {
			/// once declared exhausted, pending files are released for other modems
			this->iterate_failed_counter();
			logger::logger(__FUNCTION__, this->getInfo() + "- SMS 400| " + to_string(this->get_failed_counter()) + "/" + to_string(this->get_exhaust_count()), "stderr");

			if( this->get_failed_counter() >= this->get_exhaust_count() ) {
				/// release pending files
				this->release_pending_files();

				/// declare modem exhausted
				// this->set_modem_state(EXHAUSTED);
			}
			
			else {
				/// create pending file
				this->declare_pending( locked_request_filename );
			}
		}

		/// SMS failed due to an error
		else if( send_sms_status == "error") {
			if( !sys_calls::file_handlers( this->getConfigs()["DIR_ERROR"], sys_calls::EXIST )) {
				logger::logger(__FUNCTION__, "Error Directory");
				sys_calls::make_dir( this->getConfigs()["DIR_ERROR"] );
			}

			//TODO: Delete SMS job

			if(string locked_filename = request["filename"]; !sys_calls::rename_file(locked_filename, this->getConfigs()["DIR_ERROR"] + "/" + request["q_filename"]) and !sys_calls::rename_file(this->getConfigs()["DIR_ERROR"] + "/." + request["q_filename"], this->getConfigs()["DIR_ERROR"] + "/" + request["q_filename"])) {
				logger::logger(__FUNCTION__, this->getInfo() + " - Failed to move file to ERROR", "stderr", true); logger::logger_errno( errno );
			}
		
			else {
				logger::logger(__FUNCTION__, this->getInfo() + " - Moved file to error", "stderr", true);
			}
		}
		helpers::sleep_thread( this->get_sleep_time() );
	}
}

int Modem::get_sleep_time() const {
	return this->sleep_time;
}

int Modem::get_exhaust_count() const {
	return this->exhaust_count;
}

void Modem::set_sleep_time( int sleep_time ) {
	this->sleep_time = sleep_time;
}


string Modem::getErrorLogs() {
	return this->errorLogs;
}

map<string,string> Modem::request_job( string path_dir_request) {
	//TODO: Remove all sanitation checks from functions
	if( path_dir_request[path_dir_request.size() - 1] == '/') path_dir_request.erase(path_dir_request.size() -1, 1);
	map<string,string> request;
	logger::logger(__FUNCTION__, this->getInfo() + " - Requesting job at: " + path_dir_request);
	map<string,string> ls_returned_values;
	sys_calls::terminal_stdout(ls_returned_values, "ls -1 "+path_dir_request);	


	if((ls_returned_values.empty() or ls_returned_values.size() != 2) and (ls_returned_values.find("return")!=ls_returned_values.end() and ls_returned_values.find("data")!=ls_returned_values.end())) {
		logger::logger(__FUNCTION__, this->getInfo() + "Error with terminal command to read ISP dir");
		return request;
	}

	if(ls_returned_values["return"] == "-1") {
		//TODO: should find and change the way this information ins bring transmitted from
		//TODO: put something here to tell the error
		return request;
	}

	string filenames = ls_returned_values["data"];
	if( filenames.empty() or filenames == "" or path_dir_request.empty()) return request;
	if( filenames.find("No such file or directory") != string::npos) {
		logger::logger(__FUNCTION__, this->getInfo() + " - Seems no request dir exist yet");
		return request;
	}

	string filename = helpers::string_split(filenames, '\n')[0];
	if(filename.empty() or filename == "") {
		logger::logger(__FUNCTION__, this->getInfo() + " - Seems no request available at this time");
		return request;
	}
	if(!sys_calls::rename_file(path_dir_request + "/" + filename, path_dir_request + "/." + filename)) {
		logger::logger(__FUNCTION__, this->getInfo() + " - Failed renaming request file: "+filename, "stderr", true);
		logger::logger_errno( errno );
		return request;
	}

	string request_content = helpers::read_file(path_dir_request + "/." + filename)[0];
	if(request_content.empty()) {
		logger::logger(__FUNCTION__, this->getInfo() + " - Request file is empty... this shouldn't happen...", "stderr", true);
		return request;
	}
	request = request_distribution_listener::request_parser( request_content );
	request.insert(make_pair("q_filename", filename));
	request.insert(make_pair("filename", path_dir_request + "/." + filename));
	request.insert(make_pair("u_filename", path_dir_request + "/" + filename));
	return request;
}

// TODO: Check error message when wrong ISP
string Modem::mmcli_send_sms( string message, string number ) {
	logger::logger(__FUNCTION__, "SENDING - [" + message + "] - [" + number + "]");
	string sms_results = sys_calls::terminal_stdout(this->configs["DIR_SCRIPTS"] + "/modem_information_extraction.sh sms send \"" + message + "\" \"" + helpers::remove_char(number, ' ') + "\" " + this->getIndex());
	sms_results = helpers::to_lowercase( sms_results );
	if( sms_results.find("successfully") != string::npos || sms_results.find("success") != string::npos) return "done";
	else if( sms_results.find( "invalid" ) != string::npos ) {
		logger::logger(__FUNCTION__, this->getInfo() + " - SMS Failed log: " + sms_results, "stderr", true);
		return "error";
	}
	else {
		logger::logger(__FUNCTION__, this->getInfo() + " - SMS Failed log: " + sms_results, "stderr", true);
	}
	
	return "failed";
}

string Modem::ssh_send_sms( string message, string number ) {
	logger::logger(__FUNCTION__, "SENDING - [" + message + "] - [" + number + "]");
	string sms_results = sys_calls::terminal_stdout("ssh root@" + this->getIndex() + " -o 'ServerAliveInterval 20' \"sendsms " + number + " '" + message + "'\"" );
	//logger::logger(__FUNCTION__, sms_results);
	sms_results = helpers::to_lowercase( sms_results );
	if( sms_results.find("success") != string::npos ) return "done";  //TODO: Add a config list for possibel HTTP code 200 here
	else {
		logger::logger(__FUNCTION__, this->getInfo() + " - SMS Failed log: " + sms_results, "stderr", true);
	}

	return "failed";
}

string Modem::send_sms(string message, string number ) {
	logger::logger(__FUNCTION__, this->getInfo() + " - About to send SMS", "stdout", true);
	message = helpers::find_and_replace_substr("\\n", "\n", message);
	message = helpers::find_and_replace_substr("\\\"", "\"", message);
	message = helpers::find_and_replace_substr("'", "\'", message);

	/// Truncate each message to 150 chars and run the epxeriement
	message = message.substr(0, 150); // TODO: remove 150=> send this in as custom masterial
	// message = message.substr(0, this->get_sms_limit());
	if( this->getType() == "MMCLI") 
		return this->mmcli_send_sms( message, number);
	else if( this->getType() == "SSH") 
		return this->ssh_send_sms( message, number );
	return "failed";
}

Modem::~Modem() {
	if( this->mysqlConnection.is_init() )  {
		string unplugged_query = "UPDATE __DEKU__.MODEMS SET POWER = 'not_plugged' WHERE IMEI = '" + this->imei + "'";
		try {
			this->mysqlConnection.query( unplugged_query );
		}
		catch( std::exception& e) {
			logger::logger(__FUNCTION__, e.what());
		}
	}
}
