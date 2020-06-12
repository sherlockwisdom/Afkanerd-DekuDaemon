// Custom header files
#include "../src/sys_calls/sys_calls.hpp"
#include "../src/formatters/logger.hpp"

#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"
// #include "CppUTest/MockSupport.h"


//TODO: Write test to check if ISP_EXCHANGES work

string logger::show_state = "TESTING";

TEST_GROUP(Sys_calls) {};

TEST(Sys_calls, isp_exchange ) {
	std::string isp_unusual = "#STOPCOVID19";
	std::string isp_exchange = "MTN{COVID,62401}:ORANGE{62402}";
	std::string expected_string = "MTN";
	
	std::string isp_output = sys_calls::isp_exchange( isp_unusual, isp_exchange);

	STRCMP_EQUAL( expected_string.c_str(), isp_output.c_str() );
}

TEST(Sys_calls, terminal_stdout_void_invalid) {
	map<string,string> ls_output, ls_output1;

	string command = "ls -1 __invalid_file";
	string command1 = command + "*";

	sys_calls::terminal_stdout(ls_output, command);
	sys_calls::terminal_stdout(ls_output1, command1);

	// logger::logger(__FUNCTION__, "Return: " + ls_output["return"]);
	// logger::logger(__FUNCTION__, "Data: " + ls_output["data"]);
	// logger::logger_errno(errno);
	
	CHECK( atoi(ls_output["return"].c_str()) != 0 and atoi(ls_output1["return"].c_str()) != 0);
}

TEST(Sys_calls, terminal_stdout_void_valid) {
	map<string,string> ls_output, ls_output1;

	string command = "ls -1 *";
	string command1 = "ls -1 *.cxx";

	sys_calls::terminal_stdout(ls_output, command);
	sys_calls::terminal_stdout(ls_output1, command1);

	// logger::logger(__FUNCTION__, "Return: " + ls_output["return"]);
	// logger::logger(__FUNCTION__, "Data: " + ls_output["data"]);
	// logger::logger_errno(errno);
	
	CHECK( atoi(ls_output["return"].c_str()) == 0 and atoi(ls_output1["return"].c_str()) == 0);
}


int main( int argc, char** argv ) {
	// Testing to see how the recording works in this aspect
	return CommandLineTestRunner::RunAllTests(argc, argv);
}
