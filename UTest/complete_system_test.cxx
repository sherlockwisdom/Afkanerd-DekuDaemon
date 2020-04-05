// Custom header files
#include "../src/formatters/helpers.hpp"

#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"
// #include "CppUTest/MockSupport.h"


TEST_GROUP(Helpers) {};

TEST(Helpers, remove_char) {
	std::string input_string = "hello;;world";
	std::string expected_string = "helloworld";
	std::string output_string = helpers::remove_char( input_string, ';');

	// (expected, actual)
	STRCMP_EQUAL(expected_string.c_str(), output_string.c_str()); 
}

int main( int argc, char** argv ) {
	// Testing to see how the recording works in this aspect
	return CommandLineTestRunner::RunAllTests(argc, argv);
}

