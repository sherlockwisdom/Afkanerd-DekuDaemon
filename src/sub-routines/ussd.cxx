#include "ussd.hpp"

using namespace std;

USSD::USSD() {}

USSD::USSD( string modem_index ) {
	this->modem_index = modem_index;
}

string USSD::initiate( string command ) {
}

map<string,string> USSD::initiate_series( vector<string> commands ) {}

string USSD::respond( string command ) {}

string USSD::status() {}

bool USSD::cancel() {}
