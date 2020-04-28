// Custom header files
#include "../src/sys_calls/sys_calls.hpp"

#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"
// #include "CppUTest/MockSupport.h"


TEST_GROUP(Sys_calls) {};

TEST(Sys_calls, terminal_stdout_void) {
	map<string,string> ls_output, ls_output1;

	string command = "ls -1 __invalid_file";
	string command1 = command + "*";

	sys_calls::terminal_stdout(ls_output, command);
	sys_calls::terminal_stdout(ls_output1, command1);

	// logger::logger(__FUNCTION__, "Return: " + ls_output["return"]);
	// logger::logger(__FUNCTION__, "Data: " + ls_output["data"]);
	// logger::logger_errno(errno);
	
	CHECK( atoi(ls_output["return"].c_str()) != 0  and atoi(ls_output1["return"].c_str()) != 0);
}


int main( int argc, char** argv ) {
	// Testing to see how the recording works in this aspect
	return CommandLineTestRunner::RunAllTests(argc, argv);
}
