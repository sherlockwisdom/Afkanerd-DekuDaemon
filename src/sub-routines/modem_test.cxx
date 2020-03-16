#include "modem_listener.cxx"

int main() {
	map<string,string> configs;
	Modem modem( configs );
	modem.setIndex("2");

	vector<map<string,string>> sms_messages = modem.get_sms_messages();

	return 0;
}
