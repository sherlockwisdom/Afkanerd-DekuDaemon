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
Modem::Modem(map<string,string> configs, STATE state ) {
	this->configs = configs;
	this->state = state;

}

Modem::Modem(const Modem& modem) {
	this->imei = modem.getIMEI();
	this->isp = modem.getISP();
	this->index = modem.getIndex();
	this->configs = modem.getConfigs();
	this->state = modem.state;
	this->type = modem.getType();
	this->failed_counter = modem.get_failed_counter();
	this->sleep_time = modem.get_sleep_time();
	this->exhaust_count = modem.get_exhaust_count();
	this->mysqlConnector = modem.get_mysql_connector();
}

void Modem::setIMEI( string IMEI ) {
	this->imei = IMEI;
}

MySQL Modem::get_mysql_connector() const {
	return this->mysqlConnector;
}

int Modem::get_failed_counter() const {
	return this->failed_counter;
}

void Modem::setISP( string ISP ) {
	this->isp = ISP.find(" ") != string::npos ? helpers::split(ISP, ' ', true)[0] : ISP;
}

void Modem::set_exhaust_count( int modem_exhaust_counts ) {
	logger::logger(__FUNCTION__, this->getInfo() + " - Setting Exhaust count to: " + to_string( modem_exhaust_counts ));
	this->exhaust_count = modem_exhaust_counts;
}

void Modem::setIndex( string index ) {
	this->index = index;
	this->type = index.find("192.168.") != string::npos ? SSH : MMCLI;
}

string Modem::getIndex() const {
	return this->index.empty() ? "" : this->index;
}

string Modem::getISP() const{
	return this->isp.empty() ? "" : this->isp;
}

string Modem::getIMEI() const {
	return this->imei.empty() ? "" : this->imei;
}

string Modem::getInfo() const {
	return this->getIMEI() + "|" + this->getISP() + "|" + this->getIndex() + "|" + (this->getType() == MMCLI ? "MMCLI" : "SSH");
}

Modem::TYPE Modem::getType() const {
	return this->type;
}

Modem::operator bool() const {
	return !this->getISP().empty();
}

bool Modem::operator==( Modem modem ) const {
	//logger::logger(__FUNCTION__, this->getInfo() + " - " + modem.getInfo());
	return (
			modem.getIndex() == this->getIndex() and
			modem.getISP() == this->getISP() and
			modem.getIMEI() == this->getIMEI() and 
			modem.getType() == this->getType()
	);
}

bool Modem::operator==( Modem* modem ) const {
	return (
			modem->getIndex() == this->getIndex() and
			modem->getISP() == this->getISP() and
			modem->getIMEI() == this->getIMEI() and
			modem->getType() == this->getType()
	);
}


bool Modem::operator>( Modem modem ) const {
	return (
			this->getIMEI() > modem.getIMEI()
	);
}

bool Modem::operator<( Modem modem ) const {
	return (
			this->getIMEI() < modem.getIMEI()
	);
}

void Modem::setKeepAlive( bool keepAlive ) {
	string changedKeepAlive = keepAlive ? "true" : "false";
	//logger::logger(__FUNCTION__, this->getInfo() + " - Changing keepAlive to " + changedKeepAlive);
	this->keepAlive = keepAlive;
}

bool Modem::getKeepAlive() const {
	return this->keepAlive;
}

bool Modem::getThreadSafety() const {
	return this->thread_safety;
}

void Modem::setThreadSafety( bool thread_safety ) {
	this->thread_safety = thread_safety;
}

map<string,string> Modem::getConfigs() const {
	return this->configs;
}

map<string,string> Modem::get_sms_message( string message_index ) const {
	string terminal_respond = sys_calls::terminal_stdout( this->getConfigs()["DIR_SCRIPTS"] + "/modem_information_extraction.sh sms read_sms " + message_index + " " + this->getIndex() );	
	
	vector<string> message_body = helpers::split( terminal_respond, '\n', true);
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
		{"timestamp", timestamp}
	};
}

vector<map<string,string>> Modem::get_sms_messages() const {
	vector<map<string,string>> sms_messages;
	string terminal_respond = sys_calls::terminal_stdout( this->getConfigs()["DIR_SCRIPTS"] + "/modem_information_extraction.sh sms all " + this->getIndex() );	
	vector<string> sms_indexes = helpers::split( terminal_respond, '\n', true );
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
void modem_sms_listener ( Modem* modem ) {
	logger::logger(__FUNCTION__, "==========> MODEM SMS LISTENER | " + modem->getInfo() + " <============");
	while( 1 ) {
		logger::logger(__FUNCTION__, modem->getInfo() + " - Checking for SMS messages");
		vector<map<string,string>> sms_messages = modem->get_sms_messages();

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
				saitama::configs = modem->getConfigs();
				saitama::execute( message );

				//TODO: should delete the message once it has been executed - THIS IS VERY URGENT, CUS MODEM INFINITE LOOP
			}
		}
		else {
			logger::logger(__FUNCTION__, modem->getInfo() + " - No SMS message found!");
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

bool Modem::db_set_working_state( WORKING_STATE working_state )  {
	this->working_state = working_state;

	//TODO: Maybe MYSQL database is passed but not registered here, so keep that in mind
	this->mysqlConnector.setConnectionDetails( configs["MYSQL_SERVER"], configs["MYSQL_USER"], configs["MYSQL_PASSWORD"], configs["MYSQL_DATABASE"]);
	if( !this->mysqlConnector.connect() ) {
		logger::logger(__FUNCTION__, "Modem MYSQL connection failed", "stderr", true);
		exit( 1 );
	}

	else {
		logger::logger(__FUNCTION__, "MYSQL connection obtained!", "stdout", true);
	}

	//MysQL interaction comes in here
	string query = "UPDATE MODEMS SET __DEKU__.STATE = 'exhausted' WHERE IMEI = " + this->imei;
	map<string, vector<string>> responds = this->mysqlConnector.query( query );

	//Allows the modem connection to MySQL server, in case of db locking
	logger::logger(__FUNCTION__, "SQL Server is going away", "stderr");
	this->mysqlConnector.close();
}


void modem_request_listener( Modem* modem ) {
	//logger::logger(__FUNCTION__, modem->getInfo() + " thread started...");
	logger::logger(__FUNCTION__, "==========> MODEM REQUEST LISTENER | " + modem->getInfo() + " <============");
	modem->setKeepAlive(true);
	
	modem->setThreadSafety( true );
	while( modem->getKeepAlive() ) {
		if(blocking_mutex.try_lock() ) {
			//logger::logger(__FUNCTION__,  modem->getInfo() + " - Acquiring mutex", "stdout");
			map<string,string> request = modem->request_job( modem->getConfigs()["DIR_ISP"] + "/" + modem->getISP() );
			if( request.empty()) {
				logger::logger(__FUNCTION__, modem->getInfo() + " - No request...", "stdout", true);
				blocking_mutex.unlock();
			}
			else {
				blocking_mutex.unlock();
				//logger::logger(__FUNCTION__, modem->getInfo() + " - Got a request!", "stdout", true);
				logger::logger(__FUNCTION__, modem->getInfo() + " - locked on file: " + request["filename"]);
				//From here we can know which message went and which failed, based on the ID
				//TODO: What is an invalid message - find it so you can delete it
				if( modem->send_sms( helpers::unescape_string(request["message"], '"'), request["number"] ) ) {
					modem->reset_failed_counter();
					logger::logger(__FUNCTION__, modem->getInfo() + " - [" + request["id"] + "] SMS sent successfully!", "stdout", true);
					//DELETE FILE
					if( !sys_calls::file_handlers( modem->getConfigs()["DIR_SUCCESS"], sys_calls::EXIST )) {
						logger::logger(__FUNCTION__, "Creating success dir");
						sys_calls::make_dir( modem->getConfigs()["DIR_SUCCESS"] );
					}

					//TODO: Delete SMS job

					if(string locked_filename = request["filename"]; !sys_calls::rename_file(locked_filename, modem->getConfigs()["DIR_SUCCESS"] + "/" + request["q_filename"]) and !sys_calls::rename_file(modem->getConfigs()["DIR_SUCCESS"] + "/." + request["q_filename"], modem->getConfigs()["DIR_SUCCESS"] + "/" + request["q_filename"])) {
						logger::logger(__FUNCTION__, modem->getInfo() + " - Failed to move file to DIR_SUCCESS", "stderr", true); logger::logger_errno( errno );
					}
				
					else {
						logger::logger(__FUNCTION__, modem->getInfo() + " - Moved file to successfull", "stdout", true);
					}

					//WRITE TO LOG FILE
				}
				else {
					// TODO: Iterate a counter here, and after 3x consider the modem exhausted, send a signal here to make some changes
					modem->iterate_failed_counter();
					logger::logger(__FUNCTION__, modem->getInfo() + " - Exhaust count(" + to_string(modem->get_exhaust_count()) + ")");
					if( modem->get_failed_counter() >= modem->get_exhaust_count()
					//and 
					//modem->db_get_working_state() != Modem::EXHAUSTED 
					){
						logger::logger(__FUNCTION__, modem->getInfo() + " - Setting DB state to exhausted!");
						modem->db_set_working_state( Modem::EXHAUSTED );
					}
					logger::logger(__FUNCTION__, modem->getInfo() + " - [" + request["id"] + "] Couldn't send SMS, unlocking file", "stderr", true);
					//RELEASE FILE
					if(string unlocked_filename = request["u_filename"]; !sys_calls::rename_file(request["filename"], unlocked_filename)) {
						logger::logger(__FUNCTION__, modem->getInfo() + " - Failed to release job: ", "stderr", true);
						logger::logger_errno( errno );
					}
				}
			}
		}
		else {
			//logger::logger(__FUNCTION__, modem->getInfo() + " - Mutex locked..", "stdout");
			helpers::sleep_thread( 3 );
			continue;
		}
		helpers::sleep_thread( modem->get_sleep_time() );
	}
	modem->setThreadSafety(false);
	//logger::logger(__FUNCTION__, modem->getInfo() + " - KeepAlive died!" );
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

void Modem::start() {
	// Looking for pending request messages
	std::thread tr_modem_request_listener = std::thread(modem_request_listener, &*this);
	
	//TODO: Checks for incoming sms messages here
	//std::thread tr_modem_sms_listener = std::thread(modem_sms_listener, &*this);
	tr_modem_request_listener.join();
	//tr_modem_sms_listener.join();
}

bool Modem::end() {
	this->setKeepAlive(false);
	return true;
}

string Modem::getErrorLogs() {
	return this->errorLogs;
}

map<string,string> Modem::request_job( string path_dir_request) {
	map<string,string> request;
	logger::logger(__FUNCTION__, this->getInfo() + " - Requesting job at: " + path_dir_request);
	map<string,string> ls_returned_values;
	sys_calls::terminal_stdout(ls_returned_values, "ls -1 "+path_dir_request);	


	if((ls_returned_values.empty() or ls_returned_values.size() != 2) and (ls_returned_values.find("return")!=ls_returned_values.end() and ls_returned_values.find("data")!=ls_returned_values.end())) {
		logger::logger(__FUNCTION__, this->getInfo() + "Error with terminal command to read ISP dir");
		return request;
	}

	if(ls_returned_values["return"] == "-1") {
		//TODO: put something here to tell the error
		return request;
	}

	string filenames = ls_returned_values["data"];
	if( filenames.empty() or filenames == "" or path_dir_request.empty()) return request;
	if( filenames.find("No such file or directory") != string::npos) {
		logger::logger(__FUNCTION__, this->getInfo() + " - Seems no request dir exist yet");
		return request;
	}

	string filename = helpers::split(filenames, '\n', true)[0];
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

bool Modem::mmcli_send_sms( string message, string number ) {
	logger::logger(__FUNCTION__, "SENDING - [" + message + "] - [" + number + "]");
	string sms_results = sys_calls::terminal_stdout(this->configs["DIR_SCRIPTS"] + "/modem_information_extraction.sh sms send \"" + message + "\" \"" + helpers::remove_char_advanced(number, ' ') + "\" " + this->getIndex());
	sms_results = helpers::to_lowercase( sms_results );
	if( sms_results.find("successfully") != string::npos ) return true;
	else {
		logger::logger(__FUNCTION__, "SMS Failed log: " + sms_results, "stderr", true);
	}
	
	return false;
}

bool Modem::ssh_send_sms( string message, string number ) {
	logger::logger(__FUNCTION__, "SENDING - [" + message + "] - [" + number + "]");
	string sms_results = sys_calls::terminal_stdout("ssh root@" + this->getIndex() + " -o 'ServerAliveInterval 20' \"sendsms " + number + " '" + message + "'\"" );
	//logger::logger(__FUNCTION__, sms_results);
	sms_results = helpers::to_lowercase( sms_results );
	if( sms_results.find("success") != string::npos ) return true;  //TODO: Add a config list for possibel HTTP code 200 here
	else {
		logger::logger(__FUNCTION__, this->getInfo() + " - SMS Failed log: " + sms_results, "stderr", true);
	}
	
	return false;
}

bool Modem::send_sms(string message, string number ) {
	logger::logger(__FUNCTION__, this->getInfo() + " - About to send SMS", "stdout", true);
	message = helpers::find_and_replace("\\n", "\n", message);
	message = helpers::find_and_replace("\\\"", "\"", message);
	message = helpers::find_and_replace("'", "\'", message);
	switch( this->getType() ) {
		case Modem::MMCLI:
			return this->mmcli_send_sms( message, number);
		break;

		case Modem::SSH:
			return this->ssh_send_sms( message, number );
		break;

		default:
			logger::logger(__FUNCTION__, this->getInfo() + " - Failed to get type..", "stderr");
		break;
	}
	return false;
}

Modem::~Modem() {
	//logger::logger(__FUNCTION__, this->getInfo() + " - Destructor called...");
}
