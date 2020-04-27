// Custom header files
#include "../src/sub-routines/modem.cxx"

#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP( Modem ) {};

std::string imei = "";
std::string isp = "";
std::string type = "";
std::string index = "";
std::map<string,string> configs {};
MySQL mysqlConnection;

TEST(Modem, constructor) {

}

TEST(Modem, get_failed_counter) {
}

TEST(Modem, set_exhaust_count) {
}

TEST(Modem, set_index ) {
}

TEST(Modem, set_imei ) {
}

TEST(Modem, set_configs ) {
}

int main( int argc, char** argv ) {
	// Testing to see how the recording works in this aspect
	return CommandLineTestRunner::RunAllTests(argc, argv);
}
