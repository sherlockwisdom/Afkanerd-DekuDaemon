#include "helpers.hpp"
#include "logger.hpp"
#include "declarations.hpp"
#include "gl_request_queue_listener.cxx"
#include "gl_modem_listener.cxx"


using namespace std;


class Test {
	public:
		bool equals( auto value1, auto value2 ) {
			return value1 == value2;
		}
};


string test_request_file = string( getenv("HOME")) + "/deku/test_request.txt";

int main() {

	Test test;

	auto list_of_modems = gl_modem_listener();
	test.equals( list_of_modems.size(), 2 );
	return 0;
}
