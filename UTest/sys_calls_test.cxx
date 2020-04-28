// Custom header files
#include "../src/sys_calls/sys_calls.hpp"

#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"
// #include "CppUTest/MockSupport.h"


TEST_GROUP(Sys_calls) {};

TEST(Sys_calls, terminal_stdout_void) {
	map<string,string> ls_output;

	string command = "ls -1 __invalid_file";
	sys_calls::terminal_stdout(ls_output, command);

	logger::logger(__FUNCTION__, "Return: " + ls_output["return"]);
	logger::logger(__FUNCTION__, "Data: " + ls_output["data"]);
}


int main( int argc, char** argv ) {
	// Testing to see how the recording works in this aspect
	return CommandLineTestRunner::RunAllTests(argc, argv);
}
