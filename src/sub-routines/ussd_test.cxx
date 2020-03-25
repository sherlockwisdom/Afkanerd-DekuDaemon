#include "ussd.cxx"

using namespace std;


int main() {
	string modem_index = "8";
	map<string,string> configs {
		{ "DIR_SCRIPTS", "\"/home/sherlock/Desktop/open_source_codes/Afkanerd-DekuDaemon/scripts\"" }
	};
	// invalid input
	USSD ussd( modem_index, configs );

	// This is an extensive command - this command has some limiters
	string ussd_command = "*158*0#{Unlimitext}|1{You will not be refunded}|1{successfully cancelled}";
	multimap<string,string> responses = ussd.initiate_series( ussd_command );

	for(auto response : responses ) 
		logger::logger(__FUNCTION__, response.first + " <=> " + response.second );
	return 0;
}
