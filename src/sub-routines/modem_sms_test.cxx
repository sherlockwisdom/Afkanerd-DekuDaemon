#include "modem.cxx"

using namespace std;


int main() {
	//
	map<string,string> configs = {
		{"DIR_SCRIPTS", "/home/sherlock/Desktop/open_source_projects/Afkanerd-DekuDaemon/scripts"}
	};
	Modem modem;

	string modem_index = "2";
	modem.setIndex( modem_index );
	modem.set_configs( configs );


	auto sms_messages = modem.get_sms_messages();

	for(auto messages : sms_messages ) {
		for( auto message : messages ) {
			cout << message.first << " = " << message.second << endl;
		}
		modem.delete_sms( messages["index"] );
		
		cout << endl;
	}


	saitama::configs = configs;

	string test_command = "--:bash:-- pwd";
	saitama::execute( test_command );


	return 0;
}
