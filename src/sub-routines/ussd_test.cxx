#include "ussd.cxx"

using namespace std;


int main() {
	string modem_index = "";
	
	string static_test = "*155#";

	vector<string> dynamic_test = ["*123#", "5", "2", "6" ];

	// static_test = "Your balance..."

	// invalid input


	string modem_index = "0";
	USSD ussd( modem_index );
	
	string static_respond = ussd.initiate( static_test );
	cout << "static_respond=> " << static_respond << endl;
	cout << endl;

	map<string,string> dynamic_responds = ussd.initiate_series( dynamic_test );

	for(auto responds : dynamic_responds ) {
		cout << responds.first << "=> " << responds.second << endl;
	}


	return 0;

}
