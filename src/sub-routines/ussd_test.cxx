#include "ussd.cxx"

using namespace std;


int main() {
	string modem_index = "1";
	map<string,string> configs {
		{ "DIR_SCRIPTS", "\"/home/sherlock/Desktop/Deku Daemon/scripts\"" }
	};
	// invalid input
	USSD ussd( modem_index, configs );

	// This is an extensive command - this command has some limiters
	string ussd_command = "*158*0#{Unlimitext}|1{You will not be refunded}|1{successfully cancelled}";
	return 0;
}
