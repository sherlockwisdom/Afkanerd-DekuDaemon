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

	string request = "message=\"Hello world\\nNew line\",number=67000";
	map<string,string> parsed_request = request_parser( request );

	if(parsed_request.find("message") != parsed_request.end() and parsed_request.find("number") != parsed_request.end() and parsed_request["message"] == "Hello world\\nNew line" and parsed_request["number"] == "67000") {
		cout << "Parsed request PASSED..." << endl;
	}
	else {
		cout << "Parsed request FAILED..." << endl;
	}

	if( isp_distributor( parsed_request["message"], parsed_request["number"], configs ) != "MTN" ) {
		cout << "ISP distribution Failed..." << endl;
	}
	else {
		cout << "ISP distribution Passed..." << endl;
	}
}
