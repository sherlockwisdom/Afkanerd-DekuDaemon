// Custom header files
#include "../src/mysql/mysql.cxx"

#include "CppUTest/TestHarness_c.h"
#include "CppUTest/CommandLineTestRunner.h"


/// Important notice:
// - The variables are customized to my dev machine, a dynamic form of testing would be appreciated
// customise this before beginning the test
std::string mysqlServer = "localhost";
std::string mysqlUser = "root";
std::string mysqlPassword = "asshole";
std::string mysqlDatabase = "__DEKU__";

TEST_GROUP(Mysql) {};

TEST(Mysql, Constructor) {
	MySQL mysql(mysqlServer, mysqlUser, mysqlPassword, mysqlDatabase);

	STRCMP_EQUAL( mysqlServer.c_str(), mysql.get_server().c_str());
	STRCMP_EQUAL( mysqlUser.c_str(), mysql.get_user().c_str());
	STRCMP_EQUAL( mysqlPassword.c_str(), mysql.get_password().c_str());
	STRCMP_EQUAL( mysqlDatabase.c_str(), mysql.get_database().c_str());
}

TEST(Mysql, setConnectionDetails) {
	MySQL mysql;
	mysql.setConnectionDetails( mysqlServer, mysqlUser, mysqlPassword, mysqlDatabase );

	STRCMP_EQUAL( mysqlServer.c_str(), mysql.get_server().c_str());
	STRCMP_EQUAL( mysqlUser.c_str(), mysql.get_user().c_str());
	STRCMP_EQUAL( mysqlPassword.c_str(), mysql.get_password().c_str());
	STRCMP_EQUAL( mysqlDatabase.c_str(), mysql.get_database().c_str());
}

TEST(Mysql, connect) {
	MySQL mysql(mysqlServer, mysqlUser, mysqlPassword, mysqlDatabase);

	CHECK( mysql.connect() );
}


TEST(Mysql, create_database_query_pass) {
	MySQL mysql(mysqlServer, mysqlUser, mysqlPassword);
	CHECK( mysql.connect() );

	CHECK( mysql.set_database( database ));
}

TEST(Mysql, create_database_query_fail) {
	MySQL mysql(mysqlServer, mysqlUser, mysqlPassword);
	CHECK( mysql.connect() );

	std::string non_existent_database = "__DEKU__non_existent__";
	CHECK( mysql.set_database( non_existent_database ));
}

int main( int argc, char** argv ) {
	// Testing to see how the recording works in this aspect
	return CommandLineTestRunner::RunAllTests(argc, argv);
}
