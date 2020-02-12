#include "modem.hpp"
#include <iostream>
#include <thread>
#include <mutex>
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
}

void Modem::setIMEI( string IMEI ) {
	this->imei = IMEI;
}

void Modem::setISP( string ISP ) {
	this->isp = ISP.find(" ") != string::npos ? helpers::split(ISP, ' ', true)[0] : ISP;
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


void modem_request_listener( Modem* modem ) {
	//logger::logger(__FUNCTION__, modem->getInfo() + " thread started...");
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
				if( modem->send_sms( request["message"], request["number"] ) ) {
					logger::logger(__FUNCTION__, modem->getInfo() + " - [" + request["id"] + "] SMS sent successfully!", "stdout", true);
					//DELETE FILE
					if( !sys_calls::file_handlers( modem->getConfigs()["DIR_SUCCESS"], sys_calls::EXIST )) {
						logger::logger(__FUNCTION__, "Creating success dir");
						sys_calls::make_dir( modem->getConfigs()["DIR_SUCCESS"] );
					}

					/*
					cout << "Filename: " << request["filename"] << endl;
					cout << "U_Filename: " << request["u_filename"] << endl;
					cout << "Q_Filename: " << request["q_filename"] << endl;
					*/

					if(string locked_filename = request["filename"]; !sys_calls::rename_file(locked_filename, modem->getConfigs()["DIR_SUCCESS"] + "/" + request["q_filename"]) and !sys_calls::rename_file(modem->getConfigs()["DIR_SUCCESS"] + "/." + request["q_filename"], modem->getConfigs()["DIR_SUCCESS"] + "/" + request["q_filename"])) {
						logger::logger(__FUNCTION__, modem->getInfo() + " - Failed to move file to DIR_SUCCESS", "stderr", true);
						logger::logger_errno( errno );
					}
				
					else {
						logger::logger(__FUNCTION__, modem->getInfo() + " - Moved file to successfull", "stdout", true);
					}

					//WRITE TO LOG FILE
				}
				else {
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
		helpers::sleep_thread( 10 );
	}
	modem->setThreadSafety(false);
	//logger::logger(__FUNCTION__, modem->getInfo() + " - KeepAlive died!" );
}

void Modem::start() {
	std::thread tr_modem_request_listener = std::thread(modem_request_listener, &*this);
	tr_modem_request_listener.join();
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
	string sms_results = sys_calls::terminal_stdout(this->configs["DIR_SCRIPTS"] + "/modem_information_extraction.sh sms send \"" + message + "\" " + number + " " + this->getIndex());
	sms_results = helpers::to_lowercase( sms_results );
	if( sms_results.find("successfully") != string::npos ) return true;
	else {
		logger::logger(__FUNCTION__, "SMS Failed log: " + sms_results, "stderr", true);
	}
	
	return false;
}

bool Modem::ssh_send_sms( string message, string number ) {
	logger::logger(__FUNCTION__, "SENDING - [" + message + "] - [" + number + "]");
	string sms_results = sys_calls::terminal_stdout("ssh root@" + this->getIndex() + " -o 'ServerAliveInterval 20' 'sendsms " + number + " \"" + message + "\"'" );
	//logger::logger(__FUNCTION__, sms_results);
	sms_results = helpers::to_lowercase( sms_results );
	if( sms_results.find("success") != string::npos ) return true;  //TODO: Add a config list for possibel HTTP code 200 here
	else {
		logger::logger(__FUNCTION__, "SMS Failed log: " + sms_results, "stderr", true);
	}
	
	return false;
}

bool Modem::send_sms(string message, string number ) {
	logger::logger(__FUNCTION__, this->getInfo() + " - About to send SMS", "stderr", true);
	message = helpers::find_and_replace("\\n", "\n", message);
	message = helpers::find_and_replace("\\\"", "\"", message);
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
