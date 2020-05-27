// Custom header files
#include "../src/sub-routines/modem_listener.hpp"

#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

map<string,Modem*> Modems::available_modems;
MySQL mysqlConnection;

string logger::show_state = "TESTING";

std::string imei = "test_imei";
std::string isp = "test_isp";
std::string type = "mmcli";
std::string _index = "test_index";

std::string DIR_ISP = "/home/sherlock/Deku/Utest/isp";
std::string DIR_REQUEST_FILE = "/home/sherlock/Deku/Utest";
std::string DIR_SUCCESS = "/home/sherlock/Deku/200";
std::string DIR_ERROR = "/home/sherlock/Deku/404";
std::string DIR_SCRIPTS = "../scripts";
std::string STD_NAME_REQUEST_FILE = "request_file.txt";
std::string MYSQL_SERVER = "localhost";
std::string MYSQL_USER = "root";
std::string MYSQL_PASSWORD = "asshole";
std::string MYSQL_DATABASE = "__DEKU__";

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

TEST_GROUP( Modems ) {};
TEST_GROUP( Integration ) {};

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

TEST( Integration, db_insert_modems ) {
	// db_insert_modems( map<string,string> modems )
	// Modems STATE = Modems::TEST;
	Modems modems( configs, Modems::TEST);

	map<string, string> modem_details;
	modem_details.insert(make_pair("imei", imei));
	modem_details.insert(make_pair("index", _index));
	modem_details.insert(make_pair("isp", isp));
	modem_details.insert(make_pair("type", type));

	bool insert_into_db_state = modems.db_insert_modems( modem_details );
	
	CHECK( insert_into_db_state == true );
}

TEST( Integration, db_switch_power_modems ) {
	// db_insert_modems( map<string,string> modems )
	// Modems STATE = Modems::TEST;
	Modems modems( configs, Modems::TEST);

	map<string, string> modem_details;
	modem_details.insert(make_pair("imei", imei));
	modem_details.insert(make_pair("index", _index));
	modem_details.insert(make_pair("isp", isp));
	modem_details.insert(make_pair("type", type));

	std::string modem_state = "plugged";
	bool switch_db_states = modems.db_switch_power_modems( modem_details, modem_state );
	CHECK( switch_db_states == true );

	modem_state = "unplugged";
	switch_db_states = modems.db_switch_power_modems( modem_details, modem_state );
	CHECK( switch_db_states == true );

	// This test should fail here
	modem_state = "not_any";
	switch_db_states = modems.db_switch_power_modems( modem_details, modem_state );
	CHECK( switch_db_states == false );
}

int main( int argc, char** argv ) {
	// Testing to see how the recording works in this aspect
	return CommandLineTestRunner::RunAllTests(argc, argv);
}
