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
	helpers::write_file( test_request_file, sample_request_string, true, ios::trunc );
	vector<string> readfile = helpers::read_file( test_request_file );
	if( readfile.size() == 1) {
		cout << "Testing=> Matching number of lines..." << endl;
		if( readfile[0] == helpers::unescape_string( sample_request_string ) ) {
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
	cout << "\nTesting=> gl_request_queue_listener => " << endl;
	map<string, string > test_gl_request_queue_listener = gl_request_queue_listener( test_request_file.c_str() );
	auto it_test_gl_request_queue_listener = test_gl_request_queue_listener.begin();
	string filename = it_test_gl_request_queue_listener->first;

	size_t request_queue_size = test_gl_request_queue_listener.size();
	size_t read_tmp_file_size = helpers::read_file( filename ).size();

	if( request_queue_size != read_tmp_file_size ) {
		cout << "Testing=> Failed...." << endl;
		cout << "processed request = " << request_queue_size << endl;
		cout << "expected request = " << read_tmp_file_size << endl;
	}
	else {
		cout << "Testing=> Passed..." << endl;
		cout << "processed request = " << request_queue_size << endl;
		cout << "expected request = " << read_tmp_file_size << endl;

		if( test_gl_request_queue_listener.size() == 0 ) {
			cout << "Testing=> Failed..." << endl;
			cout << "Empty request from listener..." << endl;
		}
		else {
			cout << "\nTesting=> dequeu_from_request_file => " << endl;
			string content = it_test_gl_request_queue_listener->second;
			
			if( helpers::file_exist( filename ) ) {
				cout << "Testing=> Passed..." << endl;
				cout << "File created in tmp/ ..." << endl;
				
				if( helpers::unescape_string( content ) == helpers::unescape_string( sample_request_string) ) {
				       cout << "Testing=> Passed..." << endl;
				       cout << "Contents match test sample..." << endl;
				       //TODO: ISP Distribution
				       
				       vector< map<string,string>> dequeued_request = dequeue_from_request_file( filename ); 
				}
				else {
					cout << "Testing=> Failed..." << endl;
					cout << "Contents don't match sample..." << endl;
					cout << "Content: " << content << endl;
					cout << "Sample: " << sample_request_string << endl;
				}
			}
			else {
				cout << "Testing=> Failed.." << endl;
				cout << "File does not exist in tmp/ ..." << endl;
			}
		}
	}


	return 0;
}
