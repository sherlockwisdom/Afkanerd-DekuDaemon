#include "ussd.cxx"

using namespace std;


int main() {
	string modem_index = "0";
	map<string,string> configs {
		{ "DIR_SCRIPTS", "/home/sherlock/Desktop/Deku Daemon/scripts" }
	};
	// invalid input
	USSD ussd( modem_index, configs );

	string static_test = "*155#";
	vector<string> dynamic_test {"*123#", "5", "2", "6"};

	string static_respond = ussd.initiate( static_test );
	cout << "static_respond=> " << static_respond << endl;

	if( static_respond.find("Your balance") != string::npos) {
		cout << "PASSED" << endl;
	}
	else {
		cout << "FAILED" << endl;
	}
	cout << endl;

	map<string,string> dynamic_responds = ussd.initiate_series( dynamic_test );

	for(auto responds : dynamic_responds ) {
		cout << responds.first << "=> " << responds.second << endl;
	}

	//conditions to test dynamic input request

	return 0;
}
