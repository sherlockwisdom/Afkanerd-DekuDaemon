#include "request_distribution_listener.cxx"

using namespace std;

int main() {
	map<string,string> configs{
		{"DIR_ISP", "test_dir"},
		{"DIR_REQUEST_FILE", "test_dir"},
		{"STD_NAME_REQUEST_FILE", "request_file.txt"}
	};

	if(!configs_check( configs ) ) {
		cout << "Configs check FAILED..." << endl;
	}
	else {
		cout << "Configs check PASSED" << endl;
	}

	string sample_message = "Hello world\nNew line";
	string request = "message=\""+sample_message+"\",number=67000";
	map<string,string> parsed_request = request_parser( request );

	if(
			parsed_request.find("message") != parsed_request.end() and 
			parsed_request.find("number") != parsed_request.end() and 
			parsed_request["message"] == "\""+sample_message+"\"" and 
			parsed_request["number"] == "67000"
	) {
		cout << "Parsed request PASSED..." << endl;
	}
	else {
		cout << "Parsed request FAILED..." << endl;
		for(auto component : parsed_request ) {
			cout << component.first << "=" << component.second << endl;
			cout << component.first.size() << "=" << component.second.size() << endl;
			if(component.first == "message") cout << string("message").size() << "=" << sample_message.size() << endl;
			else if(component.first == "number") cout << string("number").size() << "=" << string("67000").size() << endl;
			cout << endl; 
		}
	}

	if( isp_distributor( parsed_request["message"], parsed_request["number"], configs ) != "MTN" ) {
		cout << "ISP distribution FAILED..." << endl;
	}
	else {
		cout << "ISP distribution PASSED..." << endl;
	}
}
