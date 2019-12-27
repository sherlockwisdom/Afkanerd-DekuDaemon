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
	
	vector<map<string,string>> modems = gl_modem_listener();
	map<string,string> modem = {{"imei","192.168.1.1"}, {"type", "ssh"}};
	modems = multi_modem_extractor( modems );
	for( auto modem : modems ) {
		cout << "==== Meta information =====" << endl;
		for( auto info : modem ) cout << info.first << " = " << info.second << endl;
	cout << boolalpha << "Has modem: " << has_modem( modem["imei"], gl_modem_listener() ) << endl;

	return 0;
}
