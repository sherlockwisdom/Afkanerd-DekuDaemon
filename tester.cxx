#include "helpers.hpp"
#include "declarations.hpp"
#include "gl_request_queue_listener.cxx"


using namespace std;


string test_request_file = string( getenv("HOME")) + "/deku/test_request.txt";
string sample_single_message = "sample request\nmessage";
string sample_single_number = "0000000";
string sample_request_string = "number=" + sample_single_number + ",message=\"" + sample_single_message + "\"";

int main() {

	helpers::make_dir( "load_distribution" );
	return 0;
}
