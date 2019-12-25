#include "helpers.hpp"
#include "logger.hpp"
#include "declarations.hpp"
#include "gl_request_queue_listener.cxx"
#include "gl_modem_listener.cxx"
#include <typeinfo>

using namespace std;

template <class Generic_Type>
class Test {
	public:
		bool equal_values( Generic_Type value1, Generic_Type value2 ) const {
			if( typeid( value1).name() != typeid( value2).name() ) return false;
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
