// Custom header files
#include "../src/sub-routines/modem.hpp"

#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP( Modem ) {};
TEST_GROUP( Modem_integration ) {};

MySQL mysqlConnection;

string logger::show_state = "TESTING";

std::string imei = "test_imei";
std::string isp = "test_isp";
std::string type = "test_type";
std::string _index = "test_index";

std::string DIR_ISP = "/home/sherlock/Deku/Utest/isp";
std::string DIR_REQUEST_FILE = "/home/sherlock/Deku/Utest";
std::string DIR_SUCCESS = "/home/sherlock/Deku/200";
std::string DIR_ERROR = "/home/sherlock/Deku/404";
std::string DIR_SCRIPTS = "../scripts";
std::string STD_NAME_REQUEST_FILE = "request_file.txt";
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

TEST(Modem, constructor) {
	Modem modem(imei, isp, type, _index, configs, mysqlConnection);

	CHECK( modem == true );
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

TEST(Modem_integration, release_pending_messages) {
	Modem modem(imei, isp, type, _index, configs);

	std::string sample_message_file = "utest_message_testing";
	std::string path = configs["DIR_ISP"] + "/" + helpers::to_uppercase(isp) + "/." + sample_message_file;
	std::string create_command = "touch " + path;
	logger::logger(__FUNCTION__, create_command);
	system(create_command.c_str());

	std::string new_filename = modem.create_pending_message( sample_message_file );
	modem.release_pending_messages();
	//TODO: Put the check here
}

TEST(Modem_integration, create_pending_message) {
	Modem modem(imei, isp, type, _index, configs);

	std::string sample_message_file = "utest_message_testing";
	std::string path = configs["DIR_ISP"] + "/" + helpers::to_uppercase(isp) + "/." + sample_message_file;
	std::string create_command = "touch " + path;
	logger::logger(__FUNCTION__, create_command);
	system(create_command.c_str());
	
	std::string new_filename = modem.create_pending_message( sample_message_file );
	//TODO: Put the check here
}

int main( int argc, char** argv ) {
	// Testing to see how the recording works in this aspect
	return CommandLineTestRunner::RunAllTests(argc, argv);
}
