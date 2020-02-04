#include "modem.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include "../sys_calls/sys_calls.hpp"
#include "request_distribution_listener.cxx"
using namespace std;
std::mutex blocking_mutex;
//class Modem
Modem::Modem(map<string,string> configs, STATE state) {
	this->configs = configs;
	this->state = state;
}

Modem::Modem(const Modem& modem) {
	this->imei = modem.getIMEI();
	this->isp = modem.getISP();
	this->index = modem.getIndex();
	this->configs = modem.getConfigs();
	this->state = modem.state;
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
	return this->getIMEI() + "|" + this->getISP() + "|" + this->getIndex();
}

Modem::operator bool() const {
	return !this->getISP().empty();
}

bool Modem::operator==( Modem modem ) const {
	logger::logger(__FUNCTION__, this->getInfo() + " - " + modem.getInfo());
	return (
			modem.getIndex() == this->getIndex() and
			modem.getISP() == this->getISP() and
			modem.getIMEI() == this->getIMEI()
	);
}

bool Modem::operator==( Modem* modem ) const {
	logger::logger(__FUNCTION__, this->getInfo() + " - " + modem->getInfo());
	return (
			modem->getIndex() == this->getIndex() and
			modem->getISP() == this->getISP() and
			modem->getIMEI() == this->getIMEI()
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
	logger::logger(__FUNCTION__, this->getInfo() + " - Changing keepAlive to " + changedKeepAlive);
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
	logger::logger(__FUNCTION__, modem->getInfo() + " thread started...");
	//cout << __FUNCTION__ << ": " << modem << endl;
	modem->setKeepAlive(true);
	//TODO: https://en.cppreference.com/w/cpp/thread/mutex
	//TODO: begin making request for task and finishing the task
	
	modem->setThreadSafety( true );
	while( modem->getKeepAlive() ) {
		//Begin making request and getting jobs back in
		if(blocking_mutex.try_lock() ) {
			logger::logger(__FUNCTION__,  modem->getInfo() + " - Acquiring mutex", "stdout");
			map<string,string> request = modem->request_job( modem->getConfigs()["DIR_ISP"] + "/" + modem->getISP() );
			if( request.empty()) {
				logger::logger(__FUNCTION__, modem->getInfo() + " - No request...", "stdout", true);
				blocking_mutex.unlock();
			}
			else {
				blocking_mutex.unlock();
				logger::logger(__FUNCTION__, modem->getInfo() + " - Got a request!", "stdout", true);
				logger::logger(__FUNCTION__, modem->getInfo() + " - locked on file: " + request["filename"]);
				if( modem->send_sms( request["message"], request["number"] ) ) {
					logger::logger(__FUNCTION__, modem->getInfo() + " - SMS sent successfully!", "stdout", true);
					//TODO: Delete file
					if( !sys_calls::file_handlers(request["filename"], sys_calls::DEL)){
						logger::logger(__FUNCTION__, modem->getInfo() + " - Failed to clean job file", "stderr", true);
						logger::logger_errno( errno );
					}
					else {
						logger::logger(__FUNCTION__, modem->getInfo() + " - Cleaned job file successfully", "stdout", true);
					}
				}
				else {
					//TODO: SMS failed to go, release the files....
					logger::logger(__FUNCTION__, modem->getInfo() + " - Couldn't send SMS, unlocking file", "stderr", true);
					if(string unlocked_filename = request["u_filename"]; !sys_calls::rename_file(request["filename"], unlocked_filename)) {
						logger::logger(__FUNCTION__, modem->getInfo() + " - Failed to release job: ", "stderr", true);
						logger::logger_errno( errno );
					}
				}
			}
		}
		else {
			logger::logger(__FUNCTION__, modem->getInfo() + " - Mutex locked..", "stdout");
			helpers::sleep_thread( 3 );
			continue;
		}
		helpers::sleep_thread( 10 );
	}
	modem->setThreadSafety(false);
	logger::logger(__FUNCTION__, modem->getInfo() + " - KeepAlive died!" );
}

void modem_state_listener( Modem& modem ) {
	//TODO: listens for changes to modems state and updates appropriately
}

void Modem::start() {
	//std::thread tr_modem_request_listener(&Modem::modem_request_listener, this, this->configs);
	//cout << __FUNCTION__ << "mem address before start: " << &*this << endl;
	std::thread tr_modem_request_listener = std::thread(modem_request_listener, &*this);
	//std::thread tr_modem_state_listener = std::thread(modem_state_listener, std::ref(*this));

	//if(this->state == TEST) tr_modem_request_listener.detach(); //TODO: change to detach
	//if(this->state == TEST) tr_modem_request_listener.join(); //TODO: change to detach
	//else if(this->state == PRODUCTION) tr_modem_request_listener.join();
	//this->request_thread_id.push_back(tr_modem_request_listener);
	tr_modem_request_listener.join();

	//if(this->state == TEST) tr_modem_state_listener.detach();
	//if(this->state == TEST) tr_modem_state_listener.join();
	//else if(this->state == PRODUCTION) tr_modem_state_listener.join();
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
	string filenames = sys_calls::terminal_stdout("ls -1 "+path_dir_request);	
	if( filenames.empty() or filenames == "" or path_dir_request.empty()) return request;
	
	string filename = helpers::split(filenames, '\n', true)[0];
	if(!sys_calls::rename_file(path_dir_request + "/" + filename, path_dir_request + "/." + filename)) {
		logger::logger(__FUNCTION__, this->getInfo() + " - Failed renaming request file (failed locking it)...", "stderr", true);
		logger::logger_errno( errno );
		return request;
	}
	string request_content = helpers::read_file(path_dir_request + "/." + filename)[0];
	if(request_content.empty()) {
		logger::logger(__FUNCTION__, this->getInfo() + " - Request file is empty... this shouldn't happen...", "stderr", true);
		return request;
	}
	request = request_distribution_listener::request_parser( request_content );
	request.insert(make_pair("filename", path_dir_request + "/." + filename));
	request.insert(make_pair("u_filename", path_dir_request + "/" + filename));
	return request;
}

bool Modem::send_sms(string message, string number ) {
	//TODO: something here to send the messages
	return false;
}

Modem::~Modem() {
	/*
	if(this->keepAlive) {
		logger::logger(__FUNCTION__, "Keeping Modem as a service...");
		std::thread tr_modem_request_listener = std::thread(modem_request_listener, std::ref(*this));
		std::thread tr_modem_state_listener = std::thread(modem_state_listener, std::ref(*this));

		//if(this->state == TEST) tr_modem_request_listener.detach(); //TODO: change to detach
		if(this->state == TEST) tr_modem_request_listener.join(); //TODO: change to detach
		else if(this->state == PRODUCTION) tr_modem_request_listener.join();

		//if(this->state == TEST) tr_modem_state_listener.detach();
		if(this->state == TEST) tr_modem_state_listener.join();
		else if(this->state == PRODUCTION) tr_modem_state_listener.join();
	}
	*/

	logger::logger(__FUNCTION__, this->getInfo() + " - Destructor called...");
}
