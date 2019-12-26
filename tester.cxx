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

	for( auto modems : gl_modem_listener() ) {
		map<string,string> modem_info = modems;
		cout << "=== Modem Meta Info ======" << endl;
		for( auto modem : modems ) {
			cout << modem.first << " = " << modem.second << endl;
		}
		cout << endl;
	}

	map<string, string> mmcli_info = {
		{ "type" , "mmcli" },
		{ "index" , "3" }
	};

	auto modem_mmcli_info = modem_extractor( mmcli_info );
	for( auto modem_info : modem_mmcli_info) {
		cout << modem_info.first << " = " << modem_info.second << endl;
	}

	map<string, string> ssh_info = {
		{ "type" , "ssh" },
		{ "index" , "192.168.1.1" }
	};

	auto modem_ssh_info = modem_extractor( ssh_info );
	for( auto modem_info : modem_ssh_info ) {
		cout << modem_info.first << " = " << modem_info.second << endl;
	}

	return 0;
}
