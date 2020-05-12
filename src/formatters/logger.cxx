#include "logger.hpp"
using namespace std;

namespace logger {

	void logger( const char* func_name, string output, string output_stream, bool show_production) {
		if( show_state == "PRODUCTION" and !show_production) return;
		if( output.empty() ) return;

		if( output_stream == "stdout" || output_stream == "STDOUT" ) {
			cout << "[logger.info] - " << func_name << "=> " << output << endl;
		}
		else if( output_stream == "stderr" || output_stream == "STDERR" ) {
			cerr << "[logger.error] - " << func_name << "=> " << output << endl;
		}

		else cerr << "[logger.error] - LOGGER DOESN'T HAVE THAT STATE YET" << endl;
	}


	void logger_errno( int t_errno ) {
		char str_error[256];
		string error_message = strerror_r( t_errno, str_error, 256);
		cout << "[logger_errno] - ERRNO: " << t_errno << endl;
		cout << "[logger_errno] - MESSAGE: " << error_message << "=> " << endl;
	}

	void logger_tester( string func_name, bool output, string output_stream ) {
		if( output_stream == "stdout" || output_stream == "STDOUT" ) {
			cout << "[tester.info] - " << func_name << "=> " << std::boolalpha << output << endl;
		}
		else if( output_stream == "stderr" || output_stream == "STDERR" ) {
			cerr << "[tester.error] - " << func_name << "=> " << std::boolalpha << output << endl;
		}

		else cerr << "[tester.error] - LOGGER DOESN'T HAVE THAT STATE YET" << endl;
	}
}

