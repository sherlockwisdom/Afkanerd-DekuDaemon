// Custom header files
#include "../src/mysql/mysql/mysql.hpp"

#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"

TEST_GROUP(Mysql) {};

TEST(Mysql, Constructor) {
	std::string mysqlServer = "localhost";
	std::string mysqlUser = "root";
	std::string mysqlPassword = "asshole";
	std::string mysqlDatabase = "__DEKU__";

	MySQL mysql(mysqlServer, mysqlUser, mysqlPassword, mysqlDatabase);

	STRCMP_EQUAL( mysqlServer.c_str(), mysql->get_server().c_str());
	STRCMP_EQUAL( mysqlUser.c_str(), mysql->get_user().c_str());
	STRCMP_EQUAL( mysqlPassword.c_str(), mysql->get_password().c_str());
	STRCMP_EQUAL( mysqlDatabase.c_str(), mysql->get_database().c_str());
}


int main( int argc, char** argv ) {
	// Testing to see how the recording works in this aspect
	return CommandLineTestRunner::RunAllTests(argc, argv);
}
