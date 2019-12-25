#include "helpers.hpp"
#include "logger.hpp"
#include "declarations.hpp"
#include "gl_request_queue_listener.cxx"
#include "gl_modem_listener.cxx"
#include <typeinfo>
#include <cassert>

using namespace std;

template <class Generic_Type>
class Test {
	public:
		bool equal_values( Generic_Type value1, Generic_Type value2 ) const {
			if( typeid( value1).name() != typeid( value2).name() ) return false;
			assert( value1 == value2 );
			return value1 == value2;
		}
};


string test_request_file = string( getenv("HOME")) + "/deku/test_request.txt";

int main() {

	SYSTEM_STATE = "TESTING";
	Test<size_t> size_t_tester;

	auto list_of_modems = gl_modem_listener();
	logger::logger_tester("Tester", size_t_tester.equal_values( list_of_modems.size(), 2 ) );
	return 0;
}
