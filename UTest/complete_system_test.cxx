// Custom header files
#include "../src/formatters/helpers.hpp"

#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"
// #include "CppUTest/MockSupport.h"


TEST_GROUP(Helpers) {};

// Testing: helpers::remove_char(string, char)
TEST(Helpers, remove_char) {
	std::string input_string = "hello;;world";
	std::string expected_string = "helloworld";
	std::string output_string = helpers::remove_char( input_string, ';');

	// (expected, actual)
	STRCMP_EQUAL(expected_string.c_str(), output_string.c_str()); 
}

// Testing: helpers::unescape_string(string, char)
TEST(Helpers, unescape_string) {
	std::string input_string = "hello\"world";
	std::string expected_string = "hello\\\"world";
	std::string output_string = helpers::unescape_string( input_string, ';');

	// (expected, actual)
	STRCMP_EQUAL(expected_string.c_str(), output_string.c_str()); 
}

// Testing: helpers::vector_to_whole_x( template vector, char = ' ')
TEST(Helpers, vector_to_whole_string) {
	char seperation_del = ' ';
	std::vector<std::string> input_vector{"hello","world"};
	std::string expected_string = "hello world";
	std::string output_string = helpers::vector_to_whole_string<string>( input_vector,seperation_del);

	// (expected, actual)
	STRCMP_EQUAL(expected_string.c_str(), output_string.c_str()); 
}

// Testing: helpers::vector_to_whole_x( template vector, char = ' ')
TEST(Helpers, vector_to_whole_numbers) {
	//char seperation_del = ' ';
	std::vector<int> input_vector{1,2};
	int expected_int = 12;
	int output_int = helpers::vector_to_whole_numbers<int>( input_vector);

	// (expected, actual)
	LONGS_EQUAL(expected_int, output_int); 
}

int main( int argc, char** argv ) {
	// Testing to see how the recording works in this aspect
	return CommandLineTestRunner::RunAllTests(argc, argv);
}

