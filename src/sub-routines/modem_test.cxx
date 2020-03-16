#include "modem_listener.cxx"

int main() {
	map<string,string> configs {
		{"DIR_SCRIPTS", "../../scripts"}
	};
	Modem modem( configs );
	modem.setIndex("3");

	vector<map<string,string>> sms_messages = modem.get_sms_messages();

	return 0;
}
