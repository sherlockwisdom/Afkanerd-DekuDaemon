// Custom header files
#include "../src/parsers/comma_equals_parser.hpp"

#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP(Parsers) {};

TEST(Parsers, comma_seperate) {
	std::string input_string = "hello,world,";
	std::vector<std::string> expected_output {"hello", "world"};
	std::vector<std::string> output_vector = parsers::comma_seperate( input_string, ',');

	// (expected, actual)
	STRCMP_EQUAL(expected_string.c_str(), output_string.c_str()); 
}


int main( int argc, char** argv ) {
	// Testing to see how the recording works in this aspect
	return CommandLineTestRunner::RunAllTests(argc, argv);
}
