// Custom header files
#include "../src/sub-routines/modem.cxx"

#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP( Modem ) {};

TEST( Modems, get_all_indexes ) {
}

TEST( Modems, get_all_isp ) {
}

TEST( Modems, get_all_imei ) {
}

TEST( Modems, find_modem_type ) {
}

TEST( Modems, get_all_modems ) {
}

TEST( Modems, get_available_modems ) {
}

TEST( Modems, get_modem_details ) {
}

int main( int argc, char** argv ) {
	// Testing to see how the recording works in this aspect
	return CommandLineTestRunner::RunAllTests(argc, argv);
}
