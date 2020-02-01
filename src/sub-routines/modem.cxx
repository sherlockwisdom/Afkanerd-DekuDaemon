#include "modem.hpp"
#include <iostream>
#include <thread>
#include "../formatters/helpers.hpp"
using namespace std;

//class Modem
Modem::Modem() {}

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

void Modem::modem_request_listener( ) {
	logger::logger(__FUNCTION__, this->getIMEI() + " thread started...");
	this->keepAlive = true;
	//TODO: https://en.cppreference.com/w/cpp/thread/mutex
	//TODO: begin making request for task and finishing the task
	
	while( this->keepAlive ) {
	}
}

void Modem::modem_state_listener( ) {
	//TODO: listens for changes to modems state and updates appropriately
}
