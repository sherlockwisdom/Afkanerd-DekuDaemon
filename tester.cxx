#include "helpers.hpp"
#include "declarations.hpp"
#include "gl_request_queue_listener.cxx"


using namespace std;


string test_request_file = string( getenv("HOME")) + "/deku/test_request.txt";

int main() {
	CURRENT_SYSTEM_STATE = "TESTING";
	//CURRENT_SYSTEM_STATE = "PRODUCTION";

	cout << "Testing=> gl_request_queue_listener => ";
	// input = filename, output = stats
	map<string, string[2] > test_gl_request_queue_listener = gl_request_queue_listener( test_request_file.c_str() );
	if( test_gl_request_queue_listener.size() != helpers::read_file( test_request_file ).size() ) {
		cout << "Failed...." << endl;
		cout << "processed request = " << test_gl_request_queue_listener.size() <<endl;
		cout << "expected request = " << helpers::read_file( test_request_file).size() << endl;
	}
	else {
		cout << "Passed..." << endl;
		cout << "processed request = " << test_gl_request_queue_listener.size() <<endl;
		cout << "expected request = " << helpers::read_file( test_request_file).size() << endl;
	}


	return 0;
}
