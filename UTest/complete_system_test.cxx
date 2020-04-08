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
	std::string input_string = "hello\"world"; // hello"world
	std::string expected_string = "hello\\\"world"; // hello\"world
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

// Testing: helpers::find_and_replace_substr( string, string, string )
TEST(Helpers, find_and_replace_substr) {
	std::string find_string = "world";
	std::string replace_string = "sherlock";
	std::string input_string = "hello world";
	std::string expected_string = "hello sherlock";
	std::string output_string = helpers::find_and_replace_substr( find_string, replace_string, input_string);
	
	// (expected, actual)
	STRCMP_EQUAL(expected_string.c_str(), output_string.c_str()); 
}

// Testing:: helpers::string_split( string, char, bool, size_t )
TEST(Helpers, str_split) {
	std::string input_string = "hello world";
	std::vector<string> expected_vector{"hello", "world"};
	char seperation_del = ' ';
	std::vector<string> output_vector = helpers::string_split( input_string, seperation_del);

	size_t expected_vector_size = expected_vector.size();
	size_t output_vector_size = output_vector.size();
	
	// (expected, actual)
	CHECK_EQUAL( expected_vector_size, output_vector_size );
	CHECK_EQUAL( expected_vector[0], output_vector[0] );
	CHECK_EQUAL( expected_vector[1], output_vector[1] );
}

// Testing: helpers::to_uppercase( string )
TEST(Helpers, to_uppercase) {
	std::string input_string = "hello world";
	std::string expected_string = "HELLO WORLD";
	std::string output_string = helpers::to_uppercase( input_string );

	STRCMP_EQUAL( expected_string.c_str(), output_string.c_str() );
}

// Testing: helpers::to_lowercase( string )
TEST(Helpers, to_lowercase) {
	std::string input_string = "HELLO WORLD";
	std::string expected_string = "hello world";
	std::string output_string = helpers::to_lowercase( input_string );

	STRCMP_EQUAL( expected_string.c_str(), output_string.c_str() );
}

int main( int argc, char** argv ) {
	// Testing to see how the recording works in this aspect
	return CommandLineTestRunner::RunAllTests(argc, argv);
}

