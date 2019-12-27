#include "helpers.hpp"
#include "logger.hpp"
#include "declarations.hpp"
#include "gl_request_queue_listener.cxx"
#include "gl_modem_listener.cxx"
#include "gl_modem_extractor.cxx"
#include <typeinfo>

using namespace std;

template <class Generic_Type>
class Test {
	private:
		string class_name = "Test";
	public:
		bool equal_values( Generic_Type value1, Generic_Type value2 ) const {
			string func_name = this->class_name + ":equal_values";
			if( typeid( value1).name() != typeid( value2).name() ) return false;
			return value1 == value2;
		}
};


string test_request_file = string( getenv("HOME")) + "/deku/test_request.txt";

int main() {

	SYSTEM_STATE = "DEVELOPMENT";
	logger::show_state = SYSTEM_STATE;
	
	vector<map<string,string>> modems = gl_modem_listeners();
	for( auto modem : modems ) {
		modem = modem_extractor( modem );
		modem_instance( modem, modems );
	}

	return 0;
}
