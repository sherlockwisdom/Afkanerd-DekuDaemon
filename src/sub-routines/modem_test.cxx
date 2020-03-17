#include "modem_listener.cxx"

int main() {
	map<string,string> configs {
		{"DIR_SCRIPTS", "../../scripts"}
	};
	Modem modem( configs );
	modem.setIndex("5");

	vector<map<string,string>> sms_messages = modem.get_sms_messages();

	for(auto sms_message : sms_messages) {
		for(auto component : sms_message) {
			logger::logger(__FUNCTION__, component.first + " -> " + component.second);
		}
		cout << endl;
	}

	return 0;
}
