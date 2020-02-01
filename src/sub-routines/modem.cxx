#include "modem.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include "../sys_calls/sys_calls.hpp"
#include "request_distribution_listener.cxx"
using namespace std;
std::mutex blocking_mutex;
//class Modem
Modem::Modem(map<string,string> configs) {
	this->configs = configs;
}

void Modem::setIMEI( string IMEI ) {
	this->imei = IMEI;
}
void Modem::setISP( string ISP ) {
	this->isp = ISP;
}
void Modem::setIndex( string index ) {
	this->index = index;
}

string Modem::getIndex() {
	return this->index.empty() ? "" : this->index;
}

string Modem::getISP() const{
	return this->isp.empty() ? "" : this->isp;
}

string Modem::getIMEI() {
	return this->imei.empty() ? "" : this->imei;
}

Modem::operator bool() const {
	return !this->getISP().empty();
}

string Modem::start() {
	std::thread tr_modem_request_listener(&Modem::modem_request_listener, this);
	std::thread tr_modem_state_listener(&Modem::modem_state_listener, this);
	tr_modem_request_listener.detach(); //TODO: change to detach
	tr_modem_state_listener.detach();
	return this->getIMEI();
}

bool Modem::end() {
	return false;
}

string Modem::getErrorLogs() {
	return this->errorLogs;
}

map<string,string> Modem::request_job( string path_dir_request) {
	map<string,string> request;
	string filenames = sys_calls::terminal_stdout("ls -1 "+path_dir_request);	
	if( filenames.empty() or filenames == "" ) return request;
	
	string filename = helpers::split(filenames, '\n', true)[0];
	if(!sys_calls::rename_file(path_dir_request + "/" + filename, path_dir_request + "/." + filename)) {
		logger::logger(__FUNCTION__, "Failed renaming request file...", "stderr", true);
		logger::logger_errno( errno );
		return request;
	}
	string request_content = helpers::read_file(path_dir_request + "/." + filename)[0];
	request = request_distribution_listener::request_parser( request_content );
	return request;
}

void Modem::modem_request_listener( ) {
	logger::logger(__FUNCTION__, this->getIMEI() + " thread started...");
	this->keepAlive = true;
	//TODO: https://en.cppreference.com/w/cpp/thread/mutex
	//TODO: begin making request for task and finishing the task
	
	while( this->keepAlive ) {
		//Begin making request and getting jobs back in
		if(blocking_mutex.try_lock() ) {
			logger::logger(__FUNCTION__, "Obtaining blocking_mutex", "stdout");
			map<string,string> request = this->request_job( this->configs["DIR_ISP_REQUEST"] );

			blocking_mutex.unlock();
			if( this->send_sms( request["message"], request["number"] ) ) {
				logger::logger(__FUNCTION__, "SMS sent successfully!", "stdout", true);
				//TODO: Delete file
				if( !sys_calls::file_handlers(request["filename"], sys_calls::DEL)){
					logger::logger(__FUNCTION__, "Failed to clean job file", "stderr", true);
					logger::logger_errno( errno );
				}
				else {
					logger::logger(__FUNCTION__, "Cleaned job file successfully", "stdout", true);
				}
			}
		}
		else {
			logger::logger(__FUNCTION__, "Mutex locked..", "stdout");
			helpers::sleep_thread( 3 );
			continue;
		}
		helpers::sleep_thread( 10 );
	}
}

void Modem::modem_state_listener( ) {
	//TODO: listens for changes to modems state and updates appropriately
}

bool Modem::send_sms(string message, string number ) {
	//TODO: something here to send the messages
}
