#include "helpers.hpp"
#include "declarations.hpp"
#include "gl_request_queue_listener.cxx"


using namespace std;


string test_request_file = string( getenv("HOME")) + "/deku/test_request.txt";
string sample_request_string = "number=0000000,message=\"sample request\nmessage\"";

int main() {
	CURRENT_SYSTEM_STATE = "TESTING";
	//CURRENT_SYSTEM_STATE = "PRODUCTION";
	cout << "Testing=> write_file => " << endl;
	helpers::write_file( test_request_file, sample_request_string, ios::trunc, true );
	vector<string> readfile = helpers::read_file( test_request_file );
	if( readfile.size() == 1) {
		cout << "Testing=> Matching number of lines..." << endl;
		if( readfile[0] == sample_request_string ) {
			cout << "Testing=> Passed...." << endl;
		}
		else {
			cout << "Testing=> Failed..." << endl;
			cout << "Request file data seems corrupt..." << endl;
		}
	}
	else {
		cout << "Testing=> Failed..." << endl;
		cout << "File has more request than request made..." << endl;
	}

	//TODO: Build a tester function, not this manual method
	cout << "Testing=> gl_request_queue_listener => " << endl;
	if( map<string, string[2] > test_gl_request_queue_listener = gl_request_queue_listener( test_request_file.c_str() ); test_gl_request_queue_listener.size() != helpers::read_file( test_request_file ).size() ) {
		cout << "Testing=> Failed...." << endl;
		cout << "processed request = " << test_gl_request_queue_listener.size() <<endl;
		cout << "expected request = " << helpers::read_file( test_request_file).size() << endl;
	}
	else {
		cout << "Testing=> Passed..." << endl;
		cout << "processed request = " << test_gl_request_queue_listener.size() <<endl;
		cout << "expected request = " << helpers::read_file( test_request_file).size() << endl;
	}


	return 0;
}
