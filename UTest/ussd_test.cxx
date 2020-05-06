// Custom header files
#include "../src/sub-routines/ussd.cxx"

#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

std::string DIR_ISP = "";
std::string DIR_REQUEST_FILE = "";
std::string DIR_SUCCESS = "";
std::string DIR_ERROR = "";
std::string DIR_SCRIPTS = "../scripts";
std::string STD_NAME_REQUEST_FILE = "";
std::string MYSQL_SERVER = "";
std::string MYSQL_USER = "";
std::string MYSQL_PASSWORD = "";
std::string MYSQL_DATABASE = "";

std::map<string,string> configs {
	{"DIR_ISP", DIR_ISP},
	{"DIR_ERROR", DIR_ERROR},
	{"DIR_SUCCESS", DIR_SUCCESS},
	{"DIR_SCRIPTS", DIR_SCRIPTS},
	{"DIR_REQUEST_FILE", DIR_REQUEST_FILE},
	{"STD_NAME_REQUEST_FILE", STD_NAME_REQUEST_FILE},
	{"MYSQL_SERVER", MYSQL_SERVER},
	{"MYSQL_USER", MYSQL_USER},
	{"MYSQL_PASSWORD", MYSQL_PASSWORD},
	{"MYSQL_DATABASE", MYSQL_DATABASE}	
};

TEST_GROUP( USSD ) {};

TEST( USSD, initiate ) {
	string command = "135*8#"; // MTN CMR USSD to extract phonenumber
	std::string modem_index = "0";

	USSD ussd( modem_index, configs );
	bool command_state = ussd.initiate( command );

	CHECK( command_state );
}

int main( int argc, char** argv ) {
	// Testing to see how the recording works in this aspect
	return CommandLineTestRunner::RunAllTests(argc, argv);
}
