// Custom header files
#include "../src/sub-routines/modem.cxx"

#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP( Modem ) {};

std::string imei = "test_imei";
std::string isp = "test_isp";
std::string type = "test_type";
std::string index = "test_index";

std::string DIR_ISP = "";
std::string DIR_REQUEST_FILE = "";
std::string DIR_SUCCESS = "";
std::string DIR_ERROR = "";
std::string DIR_SCRIPTS = "";
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
