#include "request_distribution_listener.cxx"

using namespace std;

int main() {
	map<string,string> configs{
		{"DIR_ISP", "test_dir"},
		{"DIR_REQUEST_FILE", "test_dir"},
		{"STD_NAME_REQUEST_FILE", "request_file.txt"}
	};

	if(!configs_check( configs ) ) {
		cout << "Configs check Failed..." << endl;
	}
	else {
		cout << "Configs check PASSED" << endl;
	}

	string message = "Hello world\\nNew line";
	string number = "67000";

	if( isp_distributor( message, number, configs ) != "MTN" ) {
		cout << "ISP distribution Failed..." << endl;
	}
	else {
		cout << "ISP distribution Passed..." << endl;
	}
}
